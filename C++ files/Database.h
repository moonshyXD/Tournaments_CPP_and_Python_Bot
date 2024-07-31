#ifndef ADDPARTICIPANTS_DATABASE_H
#define ADDPARTICIPANTS_DATABASE_H

#include <iostream>
#include <libpq-fe.h>
#include <spdlog/spdlog.h>
#include "AuthorizationData.h"
#include "ErrorMessages.h"
#include "SQLCommands.h"

class Database {
private:
    static PGconn *connect(const std::string &host, const std::string &port,
                           const std::string &dbname, const std::string &user, const std::string &password);

    static void createTables();

public:
    static PGconn *conn;

    Database();

    ~Database();

    static void clearGroups();

    static void clearDatabase();

};

#endif
