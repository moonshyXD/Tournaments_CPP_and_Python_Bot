#ifndef TOURNAMENTBOT_SQLCOMMANDS_H
#define TOURNAMENTBOT_SQLCOMMANDS_H

#include "Database.h"

class SQLCommands {
private:
    static void executeCommand(const std::string &command);

public:
    static void tableUsers();

    static void tableParticipants();

    static void tableResults();

    static void tableGroups();

    static void tableParticipantGroups();

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
