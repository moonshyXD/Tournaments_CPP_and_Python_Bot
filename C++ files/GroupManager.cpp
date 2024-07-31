#include "GroupManager.h"

void GroupManager::printGroup(PGresult *_command, PGresult *res, int groupId) {
    int rows = PQntuples(res);
    InterfaceLabels::printMessage("GROUP_NUMBER");
    InterfaceLabels::printMessage("AGE");
    std::cout << PQgetvalue(_command, groupId - 1, 0) << std::endl;

    for (int i = 0; i < rows; i++) {
        std::cout << i + 1 << ". " << PQgetvalue(res, i, 0) << " " << PQgetvalue(res, i, 1) << std::endl;
    }
}

void GroupManager::retrieveGroup(int groupId) {
    int tmp = 1;
    std::vector<PGresult *> requests = SQLRequests::groupRequest(groupId);
    PGresult *res = requests[0];
    PGresult *_command = requests[1];

    printGroup(_command, res, groupId);

    PQclear(res);
    PQclear(_command);
}

std::vector<std::string> GroupManager::getUniqueYearsOfBirth() {
    PGresult *res = SQLRequests::uniqueYearsRequest();

    std::vector<std::string> years;
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        years.push_back(PQgetvalue(res, i, 0));
    }

    PQclear(res);
    return years;
}

int GroupManager::createGroup(const std::string &year_of_birth) {
    PGresult *res = SQLRequests::createGroupRequest(year_of_birth);

    int groupId = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);

    return groupId;
}

std::vector<int> GroupManager::getParticipantsByYear(const std::string &year_of_birth) {
    PGresult *res = SQLRequests::participantsByYearRequest(year_of_birth);

    std::vector<int> participants;
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        participants.push_back(std::stoi(PQgetvalue(res, i, 0)));
    }

    PQclear(res);
    return participants;
}

void GroupManager::postParticipantsToGroup(const std::vector<int> &participants, int groupId) {
    for (const int participantId: participants) {
        PGresult *res = SQLRequests::participantsToGroupRequest(participantId, groupId);
        PQclear(res);
    }
}

void GroupManager::sortParticipantsIntoGroups() {
    Database::clearGroups();

    std::vector<std::string> years = getUniqueYearsOfBirth();
    for (const auto &year: years) {
        int groupId = createGroup(year);
        std::vector<int> participants = getParticipantsByYear(year);
        postParticipantsToGroup(participants, groupId);
    }
}