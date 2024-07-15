#include "Operations.h"
#include "Translate.h"

Operations::Operations(const std::string &operation) : operation(operation) {}

void Operations::clearDatabase(Database &db) {
    Database::clearDatabase();
}

void Operations::addParticipnats(char *argv[], Database &db) {
    std::string file_path = argv[7];

    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + file_path);
    }

    std::vector<Person> people;
    std::string firstname, lastname, age;
    while (file >> firstname >> lastname >> age) {
        Person participant(firstname, lastname, age);
        people.push_back(participant);
    }

    file.close();

    for (const Person &member: people) {
        Database::addParticipant(member.firstname, member.lastname, member.age);
    }
}

void Operations::addResult(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db) {
    std::vector<std::string> _argv = Translate::translateArguments(argc, argv, bannedArguments);

    std::string firstName(_argv[1]), lastName(_argv[2]), age(_argv[3]), str_result(_argv[4]), discipline(_argv[5]);
    int result = stoi(str_result);

    Database::addResult(discipline, firstName, lastName, age, result);
}


void Operations::getWinner(Database &db) {

    Database::getWinner();
}

void Operations::getGroup(char *argv[], Database &db) {
    std::string groupNumber(argv[6]);

    Database::getGroup(stoi(groupNumber));
}

void Operations::addPartcipant(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db) {
    std::vector<std::string> _argv = Translate::translateArguments(argc, argv, bannedArguments);

    std::string firstname(_argv[1]), lastname(_argv[2]), age(_argv[3]);

    Database::addParticipant(firstname, lastname, age);
}

void Operations::getPartcipants(Database &db) {
    Database::getParticipants();
}

void Operations::sortIntoGroups(Database &db) {
    Database::sortParticipantsIntoGroups();
}

void Operations::deleteParticipant(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db) {
    std::vector<std::string> _argv = Translate::translateArguments(argc, argv, bannedArguments);

    std::string firstname(_argv[1]), lastname(_argv[2]), age(_argv[3]);

    Database::deleteParticipant(firstname, lastname, age);
}

void Operations::deleteResult(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db) {
    std::vector<std::string> _argv = Translate::translateArguments(argc, argv, bannedArguments);

    std::string firstName(_argv[1]), lastName(_argv[2]), age(_argv[3]), discipline(_argv[5]);

    Database::deleteResult(discipline, firstName, lastName, age);
}

void Operations::saveToCSV(char **argv, Database &db) {
    Database::saveToCSV(argv[7]);
}

void Operations::handleEvents(int argc, char *argv[]) {
    Database db;
    std::vector<int> bannedArguments;

    if (operation == "getWinner") {
        getWinner(db);
    } else if (operation == "addParticipants") {
        addParticipnats(argv, db);
    } else if (operation == "clearDatabase") {
        clearDatabase(db);
    } else if (operation == "sortIntoGroups") {
        sortIntoGroups(db);
    } else if (operation == "addResult") {
        bannedArguments = {0, 3, 4, 6, 7, 8};
        addResult(argc, argv, bannedArguments, db);
    } else if (operation == "getGroup") {
        getGroup(argv, db);
    } else if (operation == "addParticipant") {
        bannedArguments = {0, 3, 4, 5, 6, 7, 8};
        addPartcipant(argc, argv, bannedArguments, db);
    } else if (operation == "getParticipants") {
        getPartcipants(db);
    } else if (operation == "deleteParticipant") {
        bannedArguments = {0, 3, 4, 5, 6, 7, 8};
        deleteParticipant(argc, argv, bannedArguments, db);
    } else if (operation == "deleteResult") {
        bannedArguments = {0, 3, 4, 6, 7, 8};
        deleteResult(argc, argv, bannedArguments, db);
    } else if (operation == "saveToCSV") {
        saveToCSV(argv, db);
    }
}


