#include "SQLCommands.h"

void SQLCommands::executeCommand(const std::string &command) {
    PGresult *res = PQexec(Database::conn, command.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        ErrorMessages::printError("ERROR_EXECUTING_COMMAND");
        std::cerr << PQerrorMessage(Database::conn) << std::endl;
        ErrorMessages::printError("COMMAND");
        std::cerr << command << std::endl;
        PQclear(res);
        throw std::runtime_error("Error executing command");
    }
    PQclear(res);
}

PGresult *SQLCommands::executeQuery(PGconn *conn, const std::string &query) {
    if (!conn) {
        ErrorMessages::printError("CONNECTION_IS_NOT_ESTABLISHED");
        return nullptr;
    }

    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
        ErrorMessages::printError("QUERY_EXECUTING_ERROR");
        std::cerr << PQresultErrorMessage(res) << std::endl;
        PQclear(res);
        return nullptr;
    }

    return res;
}

void SQLCommands::createTableGroups() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS groups ("
            "id SERIAL PRIMARY KEY, "
            "group_name VARCHAR(100) NOT NULL"
            ");");
}

void SQLCommands::createTableParticipants() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS participants ("
            "id SERIAL PRIMARY KEY, "
            "first_name VARCHAR(100), "
            "last_name VARCHAR(100), "
            "year_of_birth VARCHAR(100), "
            "UNIQUE(first_name, last_name, year_of_birth)"
            ");");
}

void SQLCommands::createTableParticipantGroups() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS participant_groups ("
            "participant_id INTEGER, "
            "group_id INTEGER, "
            "FOREIGN KEY (participant_id) REFERENCES participants(id) "
            "ON DELETE CASCADE ON UPDATE CASCADE, "
            "FOREIGN KEY (group_id) REFERENCES groups(id) "
            "ON DELETE CASCADE ON UPDATE CASCADE, "
            "PRIMARY KEY (participant_id, group_id)"
            ");"
    );
}


void SQLCommands::createTableResults() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS results ("
            "id SERIAL PRIMARY KEY, "
            "discipline VARCHAR(100), "
            "participant_id INTEGER, "
            "result INTEGER, "
            "FOREIGN KEY (participant_id) REFERENCES participants(id) "
            "ON DELETE CASCADE ON UPDATE CASCADE, "
            "UNIQUE(discipline, participant_id)"
            ");");
}

void SQLCommands::clearParticipantGroups() {
    executeCommand("TRUNCATE participant_groups RESTART IDENTITY CASCADE");
}

void SQLCommands::clearGroups() {
    executeCommand("TRUNCATE groups RESTART IDENTITY CASCADE");
}

void SQLCommands::dropUsers() {
    executeCommand("DROP TABLE IF EXISTS users CASCADE");
}

void SQLCommands::dropResults() {
    executeCommand("DROP TABLE IF EXISTS results CASCADE");
}

void SQLCommands::dropParticipants() {
    executeCommand("DROP TABLE IF EXISTS participants CASCADE");
}

void SQLCommands::dropGroups() {
    executeCommand("DROP TABLE IF EXISTS groups CASCADE");
}

void SQLCommands::dropParticipantGroups() {
    executeCommand("DROP TABLE IF EXISTS participant_groups CASCADE");
}
