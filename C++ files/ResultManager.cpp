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

void ResultManager::printParticipantResults(const std::vector<Participant>& participants, int degreeNumber) {
    for (const auto &participant: participants) {
        if (participant.degree == degreeNumber) {
            std::cout << participant.firstName << " " << participant.lastName << " - ";
            for (const auto &[discipline, points]: participant.disciplinePoints) {
                std::cout << discipline << ": " << points << " ";
            }
            InterfaceLabels::printMessage("TOTAL_POINTS");
            std::cout << participant.totalPoints << std::endl;
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
        InterfaceLabels::printMessage("AGE");
        std::cout << year << std::endl;
        InterfaceLabels::printMessage("FIRST_DEGREE");
        printParticipantResults(participants, 1);

        InterfaceLabels::printMessage("SECOND_DEGREE");
        printParticipantResults(participants, 2);

        InterfaceLabels::printMessage("THIRD_DEGREE");
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
                              const std::string &year_of_birth, int result) {
    PGresult *resId = SQLRequests::getParticipantId(firstName, lastName, year_of_birth);

    if (PQntuples(resId) > 0) {
        int participantId = std::stoi(PQgetvalue(resId, 0, 0));

        PGresult *resCheck = SQLRequests::checkResultRequest(discipline, participantId);
        if (PQntuples(resCheck) > 0) {
            GeneralMessages::printMessage("POST_RESULT_ERROR");
        } else {
            PGresult *resPost = SQLRequests::postResultRequest(discipline, participantId, 100);
            GeneralMessages::printMessage("POST_RESULT_SUCCESS");
            PQclear(resPost);
        }
    } else {
        GeneralMessages::printMessage("PARTICIPANT_HAS_NOT_FOUND");
    }
}

void ResultManager::deleteResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                            const std::string &year_of_birth) {
    PGresult *resId = SQLRequests::getParticipantId(firstName, lastName, year_of_birth);

    if (PQntuples(resId) > 0) {
        int participantId = std::stoi(PQgetvalue(resId, 0, 0));

        PGresult *resCheck = SQLRequests::checkResultRequest(discipline, participantId);
        if (PQntuples(resCheck) > 0) {
            PGresult *resDelete = SQLRequests::deleteResultRequest(discipline, participantId);
            GeneralMessages::printMessage("DELETE_RESULT_SUCCESS");
            PQclear(resDelete);
        } else {
            GeneralMessages::printMessage("RESULT_HAS_NOT_FOUND");
        }

        PQclear(resCheck);
    } else {
        GeneralMessages::printMessage("PARTICIPANT_HAS_NOT_FOUND");
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
