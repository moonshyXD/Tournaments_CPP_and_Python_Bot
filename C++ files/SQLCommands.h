#ifndef TOURNAMENTBOT_SQLCOMMANDS_H
#define TOURNAMENTBOT_SQLCOMMANDS_H

#include <iostream>
#include <string>
#include <libpq-fe.h>
#include "Database.h"
#include "ErrorMessages.h"

class SQLCommands {
private:
    static void executeCommand(const std::string &command);

public:
    static void createTableParticipants();

    static void createTableResults();

    static void createTableGroups();

    static void createTableParticipantGroups();

    static void clearParticipantGroups();

    static void clearGroups();

    static void dropUsers();

    static void dropResults();

    static void dropParticipants();

    static void dropGroups();

    static void dropParticipantGroups();

    static PGresult *executeQuery(PGconn *conn, const std::string &query);
};


#endif
