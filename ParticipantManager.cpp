#include "ParticipantManager.h"

void ParticipantManager::addParticipant(const std::string &firstName, const std::string &lastName,
                                        const std::string &year_of_birth) {
    PGresult *resCheck = SQLRequests::checkParticipantRequest(firstName, lastName, year_of_birth);

    bool exists = PQntuples(resCheck) > 0;
    PQclear(resCheck);

    if (!exists) {
        PGresult *resInsert = SQLRequests::addParticipantRequest(firstName, lastName, year_of_birth);

        PQclear(resInsert);
        std::cout << "Участник успешно добавлен." << std::endl;
    } else {
        std::cout << "Этот участник уже был добавлен." << std::endl;
    }
}

void ParticipantManager::deleteParticipant(const std::string &firstName, const std::string &lastName,
                                           const std::string &year_of_birth) {
    PGresult *resCheck = SQLRequests::checkParticipantRequest(firstName, lastName, year_of_birth);

    bool exists = PQntuples(resCheck) > 0;
    PQclear(resCheck);

    if (exists) {
        PGresult *resDelete = SQLRequests::deleteParticipantRequest(firstName, lastName, year_of_birth);

        PQclear(resDelete);
        std::cout << "Участник успешно удален." << std::endl;
    } else {
        std::cout << "Такого участника нет в базе данных." << std::endl;
    }
}


void ParticipantManager::getParticipants() {
    PGresult *res = SQLRequests::getParticipantsRequest();

    int rows = PQntuples(res);

    std::cout << "Участники:" << std::endl;
    std::cout<< "Имя      Фамилия     Год рождения"<< std::endl;
    for (int i = 0; i < rows; i++) {
        std::cout <<  PQgetvalue(res, i, 1) << ", "
                  << PQgetvalue(res, i, 2) << ", "
                  << PQgetvalue(res, i, 3) << std::endl;
    }

    PQclear(res);
}
