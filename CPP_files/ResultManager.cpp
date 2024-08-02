#include "ResultManager.h"

bool ResultManager::checkFirst(const std::string &discipline) {
    std::vector<std::string> disciplines = {"БЕГ"};
    for (const auto &d : disciplines) {
        if (discipline == d) {
            spdlog::debug("Discipline '{}' requires ascending order for points", discipline);
            return true;
        }
    }
    return false;
}

void ResultManager::printParticipantResults(const std::vector<Participant>& participants, int degreeNumber) {
    for (const auto &participant : participants) {
        if (participant.degree == degreeNumber) {
            std::cout << participant.firstName << " " << participant.lastName << " - ";
            for (const auto &[discipline, points] : participant.disciplinePoints) {
                std::cout << discipline << ": " << points << " ";
            }
            InterfaceLabels::printMessage("TOTAL_POINTS");
            std::cout << participant.totalPoints << std::endl;
        }
    }
    spdlog::info("Printed results for degree number: {}", degreeNumber);
}

void ResultManager::calculatePoints(std::map<int, std::vector<Participant>> &participantsByYear,
                                    const std::vector<std::string> &disciplines) {
    for (auto &[year, participants] : participantsByYear) {
        for (const auto &discipline : disciplines) {
            sortPoints(participants, discipline);
            assignPoints(participants, discipline);
        }
    }
    spdlog::info("Calculated points for disciplines");
}

void ResultManager::assignDegrees(std::map<int, std::vector<Participant>> &participantsByYear) {
    for (auto &[year, participants] : participantsByYear) {
        std::stable_sort(participants.begin(), participants.end(), [](const Participant &a, const Participant &b) {
            return a.totalPoints > b.totalPoints;
        });

        int n = participants.size();
        int degree3Count = n / 3, degree2Count = n / 3;
        degree2Count = (n % 3 == 1) ? degree2Count + 1 : (n % 3 == 2 ) ? degree2Count + 2 : degree2Count;

        for (int i = 0; i < n; ++i) {
            participants[i].degree = (i < degree3Count) ? 3 : (i < degree2Count + degree3Count) ? 2 : 1;
        }
        spdlog::info("Assigned degrees to participants by year {}", year);
    }
}

void ResultManager::printResults(const std::map<int, std::vector<Participant>> &participantsByYear) {
    for (const auto &[year, participants] : participantsByYear) {
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
    spdlog::info("Printed all results");
}

void ResultManager::sortPoints(std::vector<Participant> &participants, const std::string &discipline) {
    std::sort(participants.begin(), participants.end(), [&](const Participant &a, const Participant &b) {
        int points_a = (a.disciplinePoints.find(discipline) != a.disciplinePoints.end()) ? a.disciplinePoints.at(discipline) : 0;
        int points_b = (b.disciplinePoints.find(discipline) != b.disciplinePoints.end()) ? b.disciplinePoints.at(discipline) : 0;

        bool ascending = checkFirst(discipline);
        return ascending ? points_a < points_b : points_a > points_b;
    });
    spdlog::info("Sorted points for discipline '{}'", discipline);
}

void ResultManager::assignPoints(std::vector<Participant> &participants, const std::string &discipline) {
    int points = 1;
    for (int i = 0; i < participants.size(); ++i) {
        if (participants[i].disciplinePoints.find(discipline) != participants[i].disciplinePoints.end()) {
            if (i > 0) {
                int prev_points = (participants[i - 1].disciplinePoints.find(discipline) != participants[i - 1].disciplinePoints.end()) ? participants[i - 1].disciplinePoints.at(discipline) : 0;
                int current_points = participants[i].disciplinePoints.at(discipline);
                if (current_points != prev_points) {
                    points = i + 1;
                }
            }
            participants[i].totalPoints += points;
        }
    }
    spdlog::info("Assigned points for discipline '{}'", discipline);
}

void ResultManager::updateParticipants(std::map<int, std::vector<Participant>> &participantsByYear, PGresult *res) {
    int n = PQntuples(res);
    for (int i = 0; i < n; ++i) {
        int year = std::stoi(PQgetvalue(res, i, 0));
        int points = std::stoi(PQgetvalue(res, i, 4));
        std::string firstName = PQgetvalue(res, i, 1);
        std::string lastName = PQgetvalue(res, i, 2);
        std::string discipline = PQgetvalue(res, i, 3);

        bool found = false;
        for (auto &participant : participantsByYear[year]) {
            if (participant.firstName == firstName && participant.lastName == lastName) {
                participant.disciplinePoints[discipline] = points;
                found = true;
                break;
            }
        }
        if (!found) {
            Participant newParticipant = {firstName, lastName, {{discipline, points}}, 0, 0};
            participantsByYear[year].push_back(newParticipant);
            spdlog::info("Added new participant: {} {} for year {}", firstName, lastName, year);
        }
    }
    spdlog::info("Updated participants from database");
}

void ResultManager::postResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                               const std::string &year_of_birth, int result) {
    PGresult *resId = SQLRequests::getParticipantId(firstName, lastName, year_of_birth);

    if (PQntuples(resId) > 0) {
        int participantId = std::stoi(PQgetvalue(resId, 0, 0));

        PGresult *resCheck = SQLRequests::checkResultRequest(discipline, participantId);
        if (PQntuples(resCheck) > 0) {
            GeneralMessages::printMessage("POST_RESULT_ERROR");
            spdlog::warn("Result already exists for participant: {} {} {}", firstName, lastName, year_of_birth);
        } else {
            PGresult *resPost = SQLRequests::postResultRequest(discipline, participantId, result);
            GeneralMessages::printMessage("POST_RESULT_SUCCESS");
            spdlog::info("Posted result for participant: {} {} {}, discipline: {}, result: {}", firstName, lastName, year_of_birth, discipline, result);
            PQclear(resPost);
        }
        PQclear(resCheck);
    } else {
        GeneralMessages::printMessage("PARTICIPANT_HAS_NOT_FOUND");
        spdlog::error("Participant not found: {} {} {}", firstName, lastName, year_of_birth);
    }
    PQclear(resId);
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
            spdlog::info("Deleted result for participant: {} {} {}, discipline: {}", firstName, lastName, year_of_birth, discipline);
            PQclear(resDelete);
        } else {
            GeneralMessages::printMessage("RESULT_HAS_NOT_FOUND");
            spdlog::warn("Result not found for discipline: {} for participant: {} {} {}", discipline, firstName, lastName, year_of_birth);
        }
        PQclear(resCheck);
    } else {
        GeneralMessages::printMessage("PARTICIPANT_HAS_NOT_FOUND");
        spdlog::error("Participant not found: {} {} {}", firstName, lastName, year_of_birth);
    }
    PQclear(resId);
}

void ResultManager::determineWinner() {
    std::vector<std::string> disciplines = getDisciplines();
    auto results = SQLRequests::getResultsRequest();

    std::map<int, std::vector<Participant>> participantsByYear;
    updateParticipants(participantsByYear, results);

    calculatePoints(participantsByYear, disciplines);
    assignDegrees(participantsByYear);
    printResults(participantsByYear);
    spdlog::info("Determined winners and printed results");
}

std::vector<std::string> ResultManager::getDisciplines() {
    PGresult *res = SQLRequests::getDisciplinesRequest();

    std::vector<std::string> disciplines;
    int n = PQntuples(res);
    for (int i = 0; i < n; ++i) {
        disciplines.push_back(PQgetvalue(res, i, 0));
    }
    PQclear(res);

    spdlog::info("Fetched {} disciplines from database", n);
    return disciplines;
}
