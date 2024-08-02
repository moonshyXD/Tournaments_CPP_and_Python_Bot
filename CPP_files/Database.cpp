#include "Database.h"

PGconn *Database::conn = connect(AuthorizationData::getMessage("HOST"),
                                 AuthorizationData::getMessage("PORT"),
                                 AuthorizationData::getMessage("DATABASE_NAME"),
                                 AuthorizationData::getMessage("USER"),
                                 AuthorizationData::getMessage("PASSWORD"));

Database::Database() {
    std::string host = AuthorizationData::getMessage("HOST");
    std::string port = AuthorizationData::getMessage("PORT");
    std::string dbname = AuthorizationData::getMessage("DATABASE_NAME");
    std::string user = AuthorizationData::getMessage("USER");
    std::string password = AuthorizationData::getMessage("PASSWORD");
    conn = connect(host, port, dbname, user, password);
    if (!conn) {
        ErrorMessages::printMessage("CONNECTION_FAILED");
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
        ErrorMessages::printMessage("CONNECTION_FAILED");
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
