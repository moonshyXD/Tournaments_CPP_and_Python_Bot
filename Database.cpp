#include "Database.h"

PGconn *Database::conn = connect("localhost", "5432", "postgres", "postgres", "bobkb170307");

Database::Database() {
    std::string host = "localhost";
    std::string port = "5432";
    std::string dbname = "postgres";
    std::string user = "postgres";
    std::string password = "bobkb170307";
    conn = connect(host, port, dbname, user, password);
    if (conn) {
        if (PQsetClientEncoding(conn, "UTF8") != 0) {
            std::cerr << "Failed to set client encoding to UTF8: " << PQerrorMessage(conn) << std::endl;
            throw std::runtime_error("Failed to set client encoding to UTF8");
        }
    } else {
        throw std::runtime_error("Connection to database failed");
    }

    SQLCommands::tableGroups();
    SQLCommands::tableParticipants();
    SQLCommands::tableParticipantGroups();
    SQLCommands ::tableResults();
    SQLCommands::tableUsers();
}

PGconn *Database::connect(const std::string &host, const std::string &port, const std::string &dbname,
                          const std::string &user, const std::string &password) {
    std::string conninfo = "host=" + host + " port=" + port + " dbname=" + dbname +
                           " user=" + user + " password=" + password;

    PGconn *conn = PQconnectdb(conninfo.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    }

    return conn;
}

void Database::checkError(PGresult *res) {
    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Database error: " << PQresultErrorMessage(res) << std::endl;
        PQclear(res);
        throw std::runtime_error("Database error occurred.");
    }
}

void
Database::addParticipant(const std::string &firstName, const std::string &lastName, const std::string &year_of_birth) {
    ParticipantManager::addParticipant(firstName, lastName, year_of_birth);
}

void Database::deleteParticipant(const std::string &firstName, const std::string &lastName,
                                 const std::string &year_of_birth) {
    ParticipantManager::deleteParticipant(firstName, lastName, year_of_birth);
}

void Database::getParticipants() {
    ParticipantManager::getParticipants();
}

void Database::addResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                         const std::string &year_of_birth, int point) {
    ResultManager::addResult(discipline, firstName, lastName, year_of_birth, point);
}

void Database::deleteResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                            const std::string &year_of_birth) {
    ResultManager::deleteResult(discipline, firstName, lastName, year_of_birth);
}

void Database::sortParticipantsIntoGroups() {
    GroupManager::sortParticipantsIntoGroups();
}

void Database::getGroup(int groupId) {
    GroupManager::getGroup(groupId);
}

void Database::saveToCSV(const std::string &filename) {
    CSVManager::saveToCSV(filename);
}

void Database::getWinner() {
    ResultManager::getWinner();
}

Database::~Database() {
    PQfinish(conn);
}


void Database::clearGroups() {
    SQLCommands::clearGroups();
    SQLCommands::clearParticipantGroups();
}

void Database::clearDatabase() {
    clearGroups();
    SQLCommands::dropGroups();
    SQLCommands::dropUsers();
    SQLCommands::dropParticipants();
    SQLCommands::dropParticipantGroups();
    SQLCommands::dropResults();
}

