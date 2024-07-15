#ifndef ADDPARTICIPANTS_OPERATIONS_H
#define ADDPARTICIPANTS_OPERATIONS_H

#include "Database.h"
#include <fstream>

struct Person {
    std::string firstname;
    std::string lastname;
    std::string age;

    Person(std::string &firstname, std::string &lastname, std::string &age)
            : firstname(firstname), lastname(lastname), age(age) {};
};

class Operations {
private:
    std::string operation;

public:
    Operations(const std::string &operation);

    void handleEvents(int argc, char *argv[]);

    static void addParticipnats(char *argv[], Database &db);

    void addResult(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db);

    static void sortIntoGroups(Database &db);

    static void getWinner(Database &db);

    void getGroup(char *argv[], Database &db);

    void addPartcipant(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db);

    void getPartcipants(Database &db);

    static void clearDatabase(Database &db);

    void deleteParticipant(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db);

    void deleteResult(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db);

    void saveToCSV(char *argv[], Database &db);

};


#endif
