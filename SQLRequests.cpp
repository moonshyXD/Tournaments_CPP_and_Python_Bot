#include "SQLRequests.h"

PGresult *SQLRequests::SQLRequest(PGconn *conn, const char *command, int parameters, const char **paramValues) {
    PGresult *res = PQexecParams(Database::conn,
                                 command,
                                 parameters,
                                 NULL,
                                 paramValues,
                                 NULL,
                                 NULL,
                                 0);

    return res;
}


std::vector<PGresult *> SQLRequests::groupRequest(int groupId){
    std::string groupIdStr = std::to_string(groupId);
    const char *paramValues[1] = {groupIdStr.c_str()};
    const char *command = "SELECT p.first_name, p.last_name FROM participants p "
                          "JOIN participant_groups pg ON p.id = pg.participant_id "
                          "WHERE pg.group_id = $1";

    PGresult *res = PQexecParams(Database::conn, command, 1, nullptr, paramValues, nullptr, nullptr, 0);
    Database::checkError(res);

    PGresult *_command = PQexec(Database::conn, "SELECT group_name FROM groups");
    Database::checkError(_command);

    std::vector<PGresult *> requests = {res, _command};

    return requests;
}

PGresult * SQLRequests::uniqueYearsRequest(){
    PGresult *res = PQexec(Database::conn, "SELECT DISTINCT year_of_birth FROM participants ORDER BY year_of_birth");
    Database::checkError(res);

    return res;
}

PGresult * SQLRequests::createGroupRequest(const std::string &year_of_birth){
    const char *paramValues[1] = {year_of_birth.c_str()};
    const char *command = "INSERT INTO groups (group_name) VALUES ($1) RETURNING id";
    int parameters = 1;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValues);
    Database::checkError(res);

    return res;
}

PGresult * SQLRequests::participantsByYearRequest(const std::string &year_of_birth){
    const char *paramValues[1] = {year_of_birth.c_str()};
    const char *command = "SELECT id FROM participants WHERE year_of_birth = $1";
    int parameters = 1;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValues);
    Database::checkError(res);

    return res;
}

PGresult *SQLRequests::participantsToGroupRequest(int participantId, int groupId){
    const char *paramValues[2] = {std::to_string(participantId).c_str(), std::to_string(groupId).c_str()};
    const char *command = "INSERT INTO participant_groups (participant_id, group_id) VALUES ($1, $2)";
    int parameters = 2;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValues);
    Database::checkError(res);

    return res;
}

PGresult *SQLRequests::getResultsRequest() {
    PGresult *res = PQexec(Database::conn,
                           "SELECT year_of_birth, first_name, last_name, discipline, point FROM results");
    Database::checkError(res);
    return res;
}

PGresult *SQLRequests::getParticipantsRequest() {
    PGresult *res = PQexec(Database::conn, "SELECT * FROM participants");
    Database::checkError(res);
    return res;
}

PGresult *SQLRequests::getDisciplinesRequest() {
    PGresult *res = PQexec(Database::conn, "SELECT DISTINCT discipline FROM results");
    Database::checkError(res);
    return res;
}

PGresult *SQLRequests::checkResultRequest(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                                          const std::string &year_of_birth){
    const char *paramValuesCheck[4] = {discipline.c_str(), firstName.c_str(), lastName.c_str(), year_of_birth.c_str()};
    const char *command = "SELECT 1 FROM results WHERE discipline = $1 AND first_name = $2 AND last_name = $3 AND year_of_birth = $4";
    int parameters = 4;

    PGresult *resCheck = SQLRequest(Database::conn, command, parameters, paramValuesCheck);
    Database::checkError(resCheck);

    return resCheck;
}

PGresult *SQLRequests::addResultRequest(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                                          const std::string &year_of_birth, int point){
    const char *paramValuesInsert[5] = {discipline.c_str(), firstName.c_str(), lastName.c_str(),
                                        year_of_birth.c_str(),
                                        std::to_string(point).c_str()};
    const char* command = "INSERT INTO results (discipline, first_name, last_name, year_of_birth, point) VALUES ($1, $2, $3, $4, $5)";
    int parameters = 5;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesInsert);

    return res;
}

PGresult *SQLRequests::deleteResultRequest(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                                        const std::string &year_of_birth){
    const char *paramValuesDelete[4] = {discipline.c_str(), firstName.c_str(), lastName.c_str(),
                                        year_of_birth.c_str()};
    const char* command = "DELETE FROM results WHERE discipline = $1 AND first_name = $2 AND last_name = $3 AND year_of_birth = $4";
    int parameters = 4;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesDelete);

    return res;
}

PGresult *SQLRequests::checkParticipantRequest(const std::string &firstName, const std::string &lastName,
                                          const std::string &year_of_birth){
    const char *paramValuesCheck[3] = {firstName.c_str(), lastName.c_str(), year_of_birth.c_str()};
    const char *command = "SELECT 1 FROM participants WHERE first_name = $1 AND last_name = $2 AND year_of_birth = $3";
    int parameters = 3;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesCheck);
    Database::checkError(res);

    return res;
}

PGresult *SQLRequests::addParticipantRequest(const std::string &firstName, const std::string &lastName,
                                               const std::string &year_of_birth){
    const char *paramValuesInsert[3] = {firstName.c_str(), lastName.c_str(), year_of_birth.c_str()};
    const char* command = "INSERT INTO participants (first_name, last_name, year_of_birth) VALUES ($1, $2, $3)";
    int parameters = 3;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesInsert);

    return res;
}

PGresult *SQLRequests::deleteParticipantRequest(const std::string &firstName, const std::string &lastName,
                                             const std::string &year_of_birth){
    const char *paramValuesDelete[3] = {firstName.c_str(), lastName.c_str(), year_of_birth.c_str()};
    const char* command = "DELETE FROM participants WHERE first_name = $1 AND last_name = $2 AND year_of_birth = $3";
    int parameters = 3;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesDelete);

    return res;
}