#include "Database.h"

PGconn *Database::conn = connect("localhost", "5432", "postgres", "postgres", "bobkb170307");

Database::Database() {
    // можно сделать файл с настройками базы данных (как в питоне короче)
    // нужно ли делать такой же файл с сообщениями об ошибке?
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

    createTables();
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

Database::~Database() {
    PQfinish(conn);
}

void Database::createTables(){
    SQLCommands::createTableGroups();
    SQLCommands::createTableParticipants();
    SQLCommands::createTableParticipantGroups();
    SQLCommands::createTableResults();
    SQLCommands::createTableUsers();
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

