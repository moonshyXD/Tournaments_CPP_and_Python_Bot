#ifndef TOURNAMENTBOT_SQLREQUESTS_H
#define TOURNAMENTBOT_SQLREQUESTS_H

#include "Database.h"
#include <vector>

class SQLRequests {
public:
    static PGresult *SQLRequest(PGconn *conn, const char *command, int nParams, const char **paramValues);

    static std::vector<PGresult *> groupRequest(int groupId);

    static PGresult *uniqueYearsRequest();

    static PGresult *createGroupRequest(const std::string &year_of_birth);

    static PGresult *participantsByYearRequest(const std::string &year_of_birth);

    static PGresult *participantsToGroupRequest(int participantId, int groupId);

    static PGresult *getResultsRequest();

    static PGresult *checkResultRequest(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                       const std::string &year_of_birth);

    static PGresult *addResultRequest(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                               const std::string &year_of_birth, int point);

    static PGresult *
    deleteResultRequest(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                        const std::string &year_of_birth);

    static PGresult *getDisciplinesRequest();

    static PGresult *checkParticipantRequest(const std::string &firstName, const std::string &lastName,
                            const std::string &year_of_birth);

    static PGresult *addParticipantRequest(const std::string &firstName, const std::string &lastName,
                                           const std::string &year_of_birth);

    static PGresult *deleteParticipantRequest(const std::string &firstName, const std::string &lastName,
                             const std::string &year_of_birth);

    static PGresult *getParticipantsRequest();
};


#endif
