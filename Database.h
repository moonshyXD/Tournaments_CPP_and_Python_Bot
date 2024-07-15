#ifndef ADDPARTICIPANTS_DATABASE_H
#define ADDPARTICIPANTS_DATABASE_H

#include <iostream>
#include <libpq-fe.h>
#include "SQLCommands.h"
#include "SQLRequests.h"
#include "ParticipantManager.h"
#include "ResultManager.h"
#include "GroupManager.h"
#include "CSVManager.h"

class Database {
private:
    static PGconn *connect(const std::string &host, const std::string &port,
                           const std::string &dbname, const std::string &user, const std::string &password);

    friend class PostgreSQL_Commands;

public:
    static PGconn *conn;

    Database();

    ~Database();

    static void checkError(PGresult *res);

    static void
    addParticipant(const std::string &firstName, const std::string &lastName, const std::string &year_of_birth);

    static void
    deleteParticipant(const std::string &firstName, const std::string &lastName, const std::string &year_of_birth);

    static void getGroup(int numberOfGroup);

    static void addResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                          const std::string &year_of_birth, int point);

    static void deleteResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                             const std::string &year_of_birth);

    static void getWinner();

    static void sortParticipantsIntoGroups();

    static void clearGroups();

    static void getParticipants();

    static void clearDatabase();

    static void saveToCSV(const std::string &filename);
};

#endif
