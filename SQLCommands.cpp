#include "SQLCommands.h"

void SQLCommands::executeCommand(const std::string &command) {
    PGresult *res = PQexec(Database::conn, command.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error executing command: " << PQerrorMessage(Database::conn) << std::endl;
        std::cerr << "Command: " << command << std::endl;
        PQclear(res);
        throw std::runtime_error("Error executing command");
    }
    PQclear(res);
}

PGresult *SQLCommands::executeQuery(PGconn *conn, const std::string &query) {
    if (!conn) {
        std::cerr << "Connection is not established." << std::endl;
        return nullptr;
    }

    PGresult *res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Query execution error: " << PQresultErrorMessage(res) << std::endl;
        PQclear(res);
        return nullptr;
    }

    return res;
}

void SQLCommands::tableGroups() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS groups ("
            "id SERIAL PRIMARY KEY, "
            "group_name VARCHAR(255) NOT NULL"
            ");");
}

void SQLCommands::tableParticipants() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS participants ("
            "id SERIAL PRIMARY KEY, "
            "first_name VARCHAR(100), "
            "last_name VARCHAR(100), "
            "year_of_birth VARCHAR(15), "
            "UNIQUE(first_name, last_name, year_of_birth)"
            ");");
}

void SQLCommands::tableParticipantGroups() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS participant_groups ("
            "participant_id INTEGER REFERENCES participants(id), "
            "group_id INTEGER REFERENCES groups(id), "
            "PRIMARY KEY (participant_id, group_id)"
            ");"
    );
}

void SQLCommands::tableResults() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS results ("
            "id SERIAL PRIMARY KEY, "
            "discipline VARCHAR(100), "
            "first_name VARCHAR(100), "
            "last_name VARCHAR(100), "
            "year_of_birth VARCHAR(15), "
            "point INTEGER, "
            "UNIQUE(discipline, first_name, last_name, year_of_birth)"
            ");");
}

void SQLCommands::tableUsers() {
    executeCommand(
            "CREATE TABLE IF NOT EXISTS users ("
            "id SERIAL PRIMARY KEY, "
            "username VARCHAR(255) NOT NULL, "
            "password VARCHAR(255) NOT NULL, "
            "role VARCHAR(15) NOT NULL, "
            "UNIQUE(username)"
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
