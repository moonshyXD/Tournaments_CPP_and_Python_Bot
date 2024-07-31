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

void SQLRequests::checkError(PGresult *res) {
    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
        ErrorMessages::printError("DATABASE_ERROR");
        std::cerr << PQresultErrorMessage(res) << std::endl;
        PQclear(res);
        throw std::runtime_error("Database error occurred.");
    }
}

std::vector<PGresult *> SQLRequests::groupRequest(int groupId){
    std::string groupIdStr = std::to_string(groupId);
    const char *paramValues[1] = {groupIdStr.c_str()};
    const char *command = "SELECT p.first_name, p.last_name FROM participants p "
                          "JOIN participant_groups pg ON p.id = pg.participant_id "
                          "WHERE pg.group_id = $1";

    PGresult *res = PQexecParams(Database::conn, command, 1, nullptr, paramValues, nullptr, nullptr, 0);
    checkError(res);

    PGresult *_command = PQexec(Database::conn, "SELECT group_name FROM groups");
    checkError(_command);

    std::vector<PGresult *> requests = {res, _command};

    return requests;
}

PGresult * SQLRequests::uniqueYearsRequest(){
    PGresult *res = PQexec(Database::conn, "SELECT DISTINCT year_of_birth FROM participants ORDER BY year_of_birth");
    checkError(res);

    return res;
}

PGresult * SQLRequests::createGroupRequest(const std::string &year_of_birth){
    const char *paramValues[1] = {year_of_birth.c_str()};
    const char *command = "INSERT INTO groups (group_name) VALUES ($1) RETURNING id";
    int parameters = 1;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValues);
    checkError(res);

    return res;
}

PGresult * SQLRequests::participantsByYearRequest(const std::string &year_of_birth){
    const char *paramValues[1] = {year_of_birth.c_str()};
    const char *command = "SELECT id FROM participants WHERE year_of_birth = $1";
    int parameters = 1;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValues);
    checkError(res);

    return res;
}

PGresult *SQLRequests::participantsToGroupRequest(int participantId, int groupId){
    const char *paramValues[2] = {std::to_string(participantId).c_str(), std::to_string(groupId).c_str()};
    const char *command = "INSERT INTO participant_groups (participant_id, group_id) VALUES ($1, $2)";
    int parameters = 2;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValues);
    checkError(res);

    return res;
}

PGresult *SQLRequests::getResultsRequest() {
    PGresult *res = PQexec(Database::conn,"SELECT p.id AS participant_id, p.first_name, p.last_name, p.year_of_birth, r.discipline, r.result\n"
                                          "FROM results r\n"
                                          "JOIN participants p ON r.participant_id = p.id;");
    checkError(res);
    return res;
}

PGresult *SQLRequests::getParticipantsRequest() {
    PGresult *res = PQexec(Database::conn, "SELECT * FROM participants");
    checkError(res);
    return res;
}

PGresult *SQLRequests::getDisciplinesRequest() {
    PGresult *res = PQexec(Database::conn, "SELECT DISTINCT discipline FROM results");
    checkError(res);
    return res;
}

PGresult *SQLRequests::checkResultRequest(const std::string &discipline, int participantId){
    const char *paramValuesCheck[2] = {discipline.c_str(), std::to_string(participantId).c_str()};
    const char *command = "SELECT 1 FROM results WHERE discipline = $1 AND participant_id = $2";
    int parameters = 2;

    PGresult *resCheck = SQLRequest(Database::conn, command, parameters, paramValuesCheck);
    checkError(resCheck);

    return resCheck;
}

PGresult *SQLRequests::postResultRequest(const std::string &discipline, int participantId, int result){
    const char *paramValuesInsert[3] = {discipline.c_str(), std::to_string(participantId).c_str(), std::to_string(result).c_str()};
    const char* command = "INSERT INTO results (discipline, participant_id, point) VALUES ($1, $2, $3)";
    int parameters = 3;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesInsert);
    checkError(res);

    return res;
}

PGresult *SQLRequests::deleteResultRequest(const std::string &discipline, int participantId){
    const char *paramValuesDelete[2] = {discipline.c_str(), std::to_string(participantId).c_str()};
    const char* command = "DELETE FROM results WHERE discipline = $1 AND participant_id = $2";
    int parameters = 2;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesDelete);
    checkError(res);

    return res;
}

PGresult *SQLRequests::checkParticipantRequest(const std::string &firstName, const std::string &lastName,
                                               const std::string &year_of_birth){
    const char *paramValuesCheck[3] = {firstName.c_str(), lastName.c_str(), year_of_birth.c_str()};
    const char *command = "SELECT 1 FROM participants WHERE first_name = $1 AND last_name = $2 AND year_of_birth = $3";
    int parameters = 3;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesCheck);
    checkError(res);

    return res;
}

PGresult *SQLRequests::postParticipantRequest(const std::string &firstName, const std::string &lastName,
                                              const std::string &year_of_birth){
    const char *paramValuesInsert[3] = {firstName.c_str(), lastName.c_str(), year_of_birth.c_str()};
    const char* command = "INSERT INTO participants (first_name, last_name, year_of_birth) VALUES ($1, $2, $3)";
    int parameters = 3;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValuesInsert);
    checkError(res);

    return res;
}

PGresult *SQLRequests::deleteParticipantRequest(int participantId){
    const char *paramValuesDelete[1] = {std::to_string(participantId).c_str()};
    const char* commandParticipantGroups = "DELETE FROM participant_groups WHERE participant_id = $1";
    int parameters = 1;

    PGresult *resParticipantGroups = SQLRequest(Database::conn, commandParticipantGroups, parameters, paramValuesDelete);
    checkError(resParticipantGroups);

    const char* commandParticipant = "DELETE FROM participants WHERE id = $1";
    PGresult *resParticipant = SQLRequest(Database::conn, commandParticipant, parameters, paramValuesDelete);
    checkError(resParticipant);

    return resParticipant;
}

PGresult *SQLRequests::getParticipantId(const std::string &firstName, const std::string &lastName, const std::string &year_of_birth) {
    const char *paramValues[3] = {firstName.c_str(), lastName.c_str(), year_of_birth.c_str()};
    const char *command = "SELECT id FROM participants WHERE first_name = $1 AND last_name = $2 AND year_of_birth = $3";
    int parameters = 3;

    PGresult *res = SQLRequest(Database::conn, command, parameters, paramValues);
    checkError(res);

    return res;
}
