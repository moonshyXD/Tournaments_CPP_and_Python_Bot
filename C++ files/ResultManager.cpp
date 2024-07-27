#include "ResultManager.h"

bool ResultManager::checkFirst(const std::string &discipline) {
    std::vector<std::string> disciplines = {"БЕГ"};
    for (int i = 0; i < disciplines.size(); ++i) {
        if (discipline == disciplines[i]) {
            return true;
        }
    }
    return false;
}

void ResultManager::printParticipantResults(const std::vector<Participant> participants, int degreeNumber) {
    for (const auto &participant: participants) {
        if (participant.degree == degreeNumber) {
            std::cout << participant.firstName << " " << participant.lastName << " - ";
            for (const auto &[discipline, points]: participant.disciplinePoints) {
                std::cout << discipline << ": " << points << " ";
            }
            std::cout << "Общие очки: " << participant.totalPoints << std::endl;
        }
    }
}

void ResultManager::calculatePoints(std::map<int, std::vector<Participant>> &participantsByYear,
                                    const std::vector<std::string> &disciplines) {
    for (auto &[year, participants]: participantsByYear) {
        for (const auto &discipline: disciplines) {
            sortPoints(participants, discipline);
            assignPoints(participants, discipline);
        }
    }
}

void ResultManager::assignDegrees(std::map<int, std::vector<Participant>> &participantsByYear) {
    for (auto &[year, participants]: participantsByYear) {
        std::stable_sort(participants.begin(), participants.end(), [](const Participant &a, const Participant &b) {
            return a.totalPoints > b.totalPoints;
        });

        int n = participants.size();
        int degree3Count = n / 3, degree2Count = n / 3;
        degree2Count = (n % 3 == 1) ? degree2Count + 1 : (n % 3 == 2 ) ? degree2Count + 2 : degree2Count;

        for (int i = 0; i < n; ++i) {
            participants[i].degree = (i < degree3Count) ? 3 : (i < degree2Count + degree3Count) ? 2 : (i < n) ? 1 : 0;
        }
    }
}

void ResultManager::printResults(const std::map<int, std::vector<Participant>> &participantsByYear) {
    for (const auto &[year, participants]: participantsByYear) {
        std::cout << "Год рождения: " << year << std::endl;
        std::cout << "1 степень:" << std::endl;
        printParticipantResults(participants, 1);

        std::cout << "2 степень:" << std::endl;
        printParticipantResults(participants, 2);

        std::cout << "3 степень:" << std::endl;
        printParticipantResults(participants, 3);

        std::cout << std::endl;
    }
}

void ResultManager::sortPoints(std::vector<Participant> &participants, const std::string &discipline) {
    std::sort(participants.begin(), participants.end(), [&](const Participant &a, const Participant &b) {
        int points_a = (a.disciplinePoints.find(discipline) != a.disciplinePoints.end()) ? a.disciplinePoints.at(
                discipline) : 0;
        int points_b = (b.disciplinePoints.find(discipline) != b.disciplinePoints.end()) ? b.disciplinePoints.at(
                discipline) : 0;

        if (checkFirst(discipline)) {
            return points_a < points_b;
        } else {
            return points_a > points_b;
        }
    });
}

void ResultManager::assignPoints(std::vector<Participant> &participants, const std::string &discipline) {
    int points = 1;
    for (int i = 0; i < participants.size(); ++i) {
        if (participants[i].disciplinePoints.find(discipline) != participants[i].disciplinePoints.end()) {
            if (i > 0) {
                int prev_points = (participants[i - 1].disciplinePoints.find(discipline) !=
                                   participants[i - 1].disciplinePoints.end())
                                  ? participants[i - 1].disciplinePoints.at(discipline) : 0;
                int current_points = participants[i].disciplinePoints.at(discipline);
                if (current_points != prev_points) {
                    points = i + 1;
                }
            }
            participants[i].totalPoints += points;
        }
    }
}

void ResultManager::updateParticipants(std::map<int, std::vector<Participant>> &participantsByYear, PGresult *res) {
    int n = PQntuples(res);
    for (int i = 0; i < n; ++i) {
        int year = std::stoi(PQgetvalue(res, i, 0)), points = std::stoi(PQgetvalue(res, i, 4));
        std::string firstName = PQgetvalue(res, i, 1), lastName = PQgetvalue(res, i, 2),
                discipline = PQgetvalue(res, i, 3);

        bool found = false;
        for (auto &participant: participantsByYear[year]) {
            if (participant.firstName == firstName && participant.lastName == lastName) {
                participant.disciplinePoints[discipline] = points;
                found = true;
                break;
            }
        }
        if (!found) {
            Participant newParticipant = {firstName, lastName, {{discipline, points}}, 0, 0};
            participantsByYear[year].push_back(newParticipant);
        }
    }
}

void ResultManager::postResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                              const std::string &year_of_birth, int point) {
    PGresult *resCheck = SQLRequests::checkResultRequest(discipline, firstName, lastName, year_of_birth);

    bool exists = PQntuples(resCheck) > 0;
    PQclear(resCheck);

    if (!exists) {
        PGresult *resInsert = SQLRequests::postResultRequest(discipline, firstName, lastName, year_of_birth, point);

        PQclear(resInsert);
        std::cout << "Результат успешно добавлен." << std::endl;
    } else {
        std::cout
                << "Результат этого участника уже записан. Для изменения результата удалите предыдущий и добавьте новый."
                << std::endl;
    }
}

void ResultManager::deleteResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                            const std::string &year_of_birth) {
    PGresult *resCheck = SQLRequests::checkResultRequest(discipline, firstName, lastName, year_of_birth);

    bool exists = PQntuples(resCheck) > 0;
    PQclear(resCheck);
    if (exists) {
        PGresult *res = SQLRequests::deleteResultRequest(discipline, firstName, lastName, year_of_birth);

        PQclear(res);
        std::cout << "Результат участника успешно удален" << std::endl;
    } else {
        std::cout << "Такого результата у участника нет в базе данных" << std::endl;
    }
}

void ResultManager::determineWinner() {
    std::vector<std::string> disciplines = getDisciplines();
    auto results = SQLRequests::getResultsRequest();

    std::map<int, std::vector<Participant>> participantsByYear;
    updateParticipants(participantsByYear, results);

    calculatePoints(participantsByYear, disciplines);
    assignDegrees(participantsByYear);
    printResults(participantsByYear);
}

std::vector<std::string> ResultManager::getDisciplines() {
    PGresult *res = SQLRequests::getDisciplinesRequest();

    std::vector<std::string> disciplines;
    int n = PQntuples(res);
    for (int i = 0; i < n; ++i) {
        disciplines.push_back(PQgetvalue(res, i, 0));
    }
    PQclear(res);

    return disciplines;
}
