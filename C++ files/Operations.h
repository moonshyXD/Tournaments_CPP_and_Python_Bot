#ifndef ADDPARTICIPANTS_OPERATIONS_H
#define ADDPARTICIPANTS_OPERATIONS_H

#include "Operation.h"
#include "Database.h"
#include "ParticipantManager.h"
#include "ResultManager.h"
#include "GroupManager.h"
#include "Translate.h"
#include "ErrorMessages.h"
#include "CSVManager.h"
#include <fstream>
#include <vector>
#include <string>

class Operations {
private:
    Operation operation;

    Operation stringToOperation(const std::string &operation);

public:
    void handleEvents(int argc, char *argv[]);

    static void loadParticipantsFromFile(char *argv[], Database &db);

    void postResult(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db);

    static void sortIntoGroups(Database &db);

    static void determineWinner(Database &db);

    void retrieveGroup(char *argv[], Database &db);

    void postPartcipant(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db);

    void retrievePartcipants(Database &db);

    static void clearDatabase(Database &db);

    void deleteParticipant(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db);

    void deleteResult(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db);

    void saveToCSV(char *argv[], Database &db);
};


#endif
