#include "Database.h"

PGconn *Database::conn = connect(AuthorizationData::getData("HOST"),
                                 AuthorizationData::getData("PORT"),
                                 AuthorizationData::getData("DATABASE_NAME"),
                                 AuthorizationData::getData("USER"),
                                 AuthorizationData::getData("PASSWORD"));

Database::Database() {
    std::string host = AuthorizationData::getData("HOST");
    std::string port = AuthorizationData::getData("PORT");
    std::string dbname = AuthorizationData::getData("DATABASE_NAME");
    std::string user = AuthorizationData::getData("USER");
    std::string password = AuthorizationData::getData("PASSWORD");
    conn = connect(host, port, dbname, user, password);
    if (!conn) {
        ErrorMessages::printError("CONNECTION_FAILED");
        spdlog::error("Database connection failed");
    } else {
        spdlog::info("Database connected successfully");
    }

    createTables();
}

PGconn *Database::connect(const std::string &host, const std::string &port, const std::string &dbname,
                          const std::string &user, const std::string &password) {
    std::string conninfo = "host=" + host + " port=" + port + " dbname=" + dbname +
                           " user=" + user + " password=" + password;

    PGconn *conn = PQconnectdb(conninfo.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        ErrorMessages::printError("CONNECTION_FAILED");
        spdlog::error("Connection failed", PQerrorMessage(conn));
        PQfinish(conn);
        return nullptr;
    }

    return conn;
}

Database::~Database() {
    PQfinish(conn);
    spdlog::info("Database connection closed");
}

void Database::createTables() {
    SQLCommands::createTableGroups();
    SQLCommands::createTableParticipants();
    SQLCommands::createTableParticipantGroups();
    SQLCommands::createTableResults();
    spdlog::info("Database tables created");
}

void Database::clearGroups() {
    SQLCommands::clearGroups();
    SQLCommands::clearParticipantGroups();
    spdlog::info("Groups cleared");
}

void Database::clearDatabase() {
    clearGroups();
    SQLCommands::dropGroups();
    SQLCommands::dropUsers();
    SQLCommands::dropParticipants();
    SQLCommands::dropParticipantGroups();
    SQLCommands::dropResults();
    spdlog::info("Database cleared");
}
