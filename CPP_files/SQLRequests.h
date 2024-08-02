#ifndef TOURNAMENTBOT_SQLREQUESTS_H
#define TOURNAMENTBOT_SQLREQUESTS_H

#include <libpq-fe.h>
#include <vector>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include "Database.h"
#include "ErrorMessages.h"

class SQLRequests {
public:
    static PGresult *SQLRequest(PGconn *conn, const char *command, int nParams, const char **paramValues);

    static std::vector<PGresult *> groupRequest(int groupId);

    static PGresult *uniqueYearsRequest();

    static PGresult *createGroupRequest(const std::string &year_of_birth);

    static PGresult *participantsByYearRequest(const std::string &year_of_birth);

    static PGresult *participantsToGroupRequest(int participantId, int groupId);

    static PGresult *getResultsRequest();

    static PGresult *checkResultRequest(const std::string &discipline, int participantId);

    static PGresult *postResultRequest(const std::string &discipline, int participantId, int result);

    static PGresult *
    deleteResultRequest(const std::string &discipline, int participantId);

    static PGresult *getDisciplinesRequest();

    static PGresult *checkParticipantRequest(const std::string &firstName, const std::string &lastName,
                            const std::string &year_of_birth);

    static PGresult *postParticipantRequest(const std::string &firstName, const std::string &lastName,
                                           const std::string &year_of_birth);

    static PGresult *deleteParticipantRequest(int participantId);

    static PGresult *getParticipantsRequest();


    static void checkError(PGresult *res);

    static PGresult *
    getParticipantId(const std::string &firstName, const std::string &lastName, const std::string &year_of_birth);
};


#endif
