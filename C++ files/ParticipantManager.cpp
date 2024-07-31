#include "ParticipantManager.h"

void ParticipantManager::postParticipant(const std::string &firstName, const std::string &lastName,
                                        const std::string &year_of_birth) {
    PGresult *resCheck = SQLRequests::checkParticipantRequest(firstName, lastName, year_of_birth);

    bool exists = PQntuples(resCheck) > 0;
    PQclear(resCheck);

    if (!exists) {
        PGresult *resInsert = SQLRequests::postParticipantRequest(firstName, lastName, year_of_birth);

        PQclear(resInsert);
        GeneralMessages::printMessage("POST_PARTICIPANT_SUCCESS");
    } else {
        GeneralMessages::printMessage("POST_PARTICIPANT_ERROR");
    }
}

void ParticipantManager::deleteParticipant(const std::string &firstName, const std::string &lastName,
                                           const std::string &year_of_birth) {
    PGresult *resId = SQLRequests::getParticipantId(firstName, lastName, year_of_birth);

    if(PQntuples(resId) > 0) {
        int participantId = std::stoi(PQgetvalue(resId, 0, 0));

        PGresult *resDelete = SQLRequests::deleteParticipantRequest(participantId);

        PQclear(resDelete);
        GeneralMessages::printMessage("DELETE_PARTICIPANT_SUCCESS");
    }else {
        GeneralMessages::printMessage("PARTICIPANT_HAS_NOT_FOUND");
    }
}

void ParticipantManager::printParticipants() {
    PGresult *res = SQLRequests::getParticipantsRequest();

    int rows = PQntuples(res);

    InterfaceLabels::printMessage("PARTICIPANTS");
    InterfaceLabels::printMessage("FIRST_NAME_LAST_NAME_AGE");
    for (int i = 0; i < rows; i++) {
        std::cout <<  PQgetvalue(res, i, 1) << " "
                  << PQgetvalue(res, i, 2) << " "
                  << PQgetvalue(res, i, 3) << std::endl;
    }

    PQclear(res);
}