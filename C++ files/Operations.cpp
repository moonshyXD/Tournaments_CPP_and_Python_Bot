#include "Operations.h"
#include "Translate.h"

void Operations::clearDatabase(Database &db) {
    Database::clearDatabase();
}

void Operations::loadParticipantsFromFile(char *argv[], Database &db) {
    std::string file_path = argv[7];

    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + file_path);
    }

    std::string firstname, lastname, age;
    while (file >> firstname >> lastname >> age) {
        ParticipantManager::postParticipant(firstname, lastname, age);
    }

    file.close();
}

void Operations::postResult(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db) {
    std::vector<std::string> _argv = Translate::translateArguments(argc, argv, bannedArguments);

    std::string firstName(_argv[1]), lastName(_argv[2]), age(_argv[3]), str_result(_argv[4]), discipline(_argv[5]);
    int result = stoi(str_result);

    ResultManager::postResult(discipline, firstName, lastName, age, result);
}


void Operations::determineWinner(Database &db) {

    ResultManager::determineWinner();
}

void Operations::retrieveGroup(char *argv[], Database &db) {
    std::string groupNumber(argv[6]);

    GroupManager::retrieveGroup(stoi(groupNumber));
}

void Operations::postPartcipant(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db) {
    std::vector<std::string> _argv = Translate::translateArguments(argc, argv, bannedArguments);

    std::string firstname(_argv[1]), lastname(_argv[2]), age(_argv[3]);

    ParticipantManager::postParticipant(firstname, lastname, age);
}

void Operations::retrievePartcipants(Database &db) {
    ParticipantManager::printParticipants();
}

void Operations::sortIntoGroups(Database &db) {
    GroupManager::sortParticipantsIntoGroups();
}

void Operations::deleteParticipant(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db) {
    std::vector<std::string> _argv = Translate::translateArguments(argc, argv, bannedArguments);

    std::string firstname(_argv[1]), lastname(_argv[2]), age(_argv[3]);

    ParticipantManager::deleteParticipant(firstname, lastname, age);
}

void Operations::deleteResult(int argc, char *argv[], const std::vector<int> &bannedArguments, Database &db) {
    std::vector<std::string> _argv = Translate::translateArguments(argc, argv, bannedArguments);

    std::string firstName(_argv[1]), lastName(_argv[2]), age(_argv[3]), discipline(_argv[5]);

    ResultManager::deleteResult(discipline, firstName, lastName, age);
}

void Operations::saveToCSV(char **argv, Database &db) {
    CSVManager::saveToCSV(argv[7]);
}

Operation Operations::stringToOperation(const std::string &operation) {
    if (operation == "getWinner") {
        return DETERMINE_WINNER;
    }
    if (operation == "addParticipants") {
        return LOAD_PARTICIPANTS_FROM_FILE;
    }
    if (operation == "clearDatabase") {
        return CLEAR_DATABASE;
    }
    if (operation == "sortIntoGroups") {
        return SORT_INTO_GROUPS;
    }
    if (operation == "addResult") {
        return POST_RESULT;
    }
    if (operation == "getGroup") {
        return RETRIEVE_GROUP;
    }
    if (operation == "addParticipant") {
        return POST_PARTICIPANT;
    }
    if (operation == "getParticipants") {
        return RETRIEVE_PARTICIPANTS;
    }
    if (operation == "deleteParticipant") {
        return DELETE_PARTICIPANT;
    }
    if (operation == "deleteResult") {
        return DELETE_RESULT;
    }
    if (operation == "saveToCSV") {
        return SAVE_TO_CSV;
    }
    return UNKNOWN_COMMAND;
}

void Operations::handleEvents(int argc, char *argv[]) {
    Database db;
    std::vector<int> bannedArguments;
    operation = stringToOperation(argv[8]);

    switch (operation) {
        case DETERMINE_WINNER:
            determineWinner(db);
            break;
        case LOAD_PARTICIPANTS_FROM_FILE:
            loadParticipantsFromFile(argv, db);
            break;
        case CLEAR_DATABASE:
            clearDatabase(db);
            break;
        case SORT_INTO_GROUPS:
            sortIntoGroups(db);
            break;
        case POST_RESULT:
            bannedArguments = {0, 3, 4, 6, 7, 8};
            postResult(argc, argv, bannedArguments, db);
            break;
        case RETRIEVE_GROUP:
            retrieveGroup(argv, db);
            break;
        case POST_PARTICIPANT:
            bannedArguments = {0, 3, 4, 5, 6, 7, 8};
            postPartcipant(argc, argv, bannedArguments, db);
            break;
        case RETRIEVE_PARTICIPANTS:
            retrievePartcipants(db);
            break;
        case DELETE_PARTICIPANT:
            bannedArguments = {0, 3, 4, 5, 6, 7, 8};
            deleteParticipant(argc, argv, bannedArguments, db);
            break;
        case DELETE_RESULT:
            bannedArguments = {0, 3, 4, 6, 7, 8};
            deleteResult(argc, argv, bannedArguments, db);
            break;
        case SAVE_TO_CSV:
            saveToCSV(argv, db);
            break;
        default:
            std::cerr << "Unknown operation: " << operation << std::endl;
            break;
    }
}


