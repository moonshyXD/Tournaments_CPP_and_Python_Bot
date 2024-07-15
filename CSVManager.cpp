#include "CSVManager.h"

std::string CSVManager::escapeQuotes(const char *str) {
    std::string result(str);
    size_t pos = result.find("\"");
    while (pos != std::string::npos) {
        result.replace(pos, 1, "\"\"");
        pos = result.find("\"", pos + 2);
    }
    return result;
}

void CSVManager::writeHeaders(std::ofstream &file, const std::vector<std::string> &headers) {
    for (size_t i = 0; i < headers.size(); ++i) {
        file << headers[i];
        if (i != headers.size() - 1) {
            file << ", ";
        }
    }
    file << std::endl;
}

void CSVManager::writeRows(std::ofstream &file, PGresult *res) {
    int rows = PQntuples(res);
    int fields = PQnfields(res);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < fields; ++j) {
            std::string value = escapeQuotes(PQgetvalue(res, i, j));
            file << "\"" << value << "\"";
            if (j != fields - 1) {
                file << ", ";
            }
        }
        file << std::endl;
    }
}

void CSVManager::saveParticipants(std::ofstream &file) {
    file << "Участники:" << std::endl;

    writeHeaders(file, {"Имя", "Фамилия", "Год рождения"});

    PGresult *resParticipants = SQLCommands::executeQuery(Database::conn,"SELECT first_name, last_name, year_of_birth FROM participants");
    writeRows(file, resParticipants);
    PQclear(resParticipants);
}

void CSVManager::saveParticipantResults(std::ofstream &file, const std::vector<Participant> &participants,
                                        int degreeNumber) {
    for (auto it = participants.rbegin(); it != participants.rend(); ++it) {
        const auto &participant = *it;
        if (participant.degree == degreeNumber) {
            file << participant.firstName << " " << participant.lastName << " - ";
            for (const auto &[discipline, points]: participant.disciplinePoints) {
                file << discipline << ": " << points << " ";
            }
            file << "Общие очки: " << participant.totalPoints << std::endl;
        }
    }

}

void CSVManager::saveResults(std::ofstream &file) {
    file << "Результаты:" << std::endl;
    file << "Имя, Фамилия, Дисциплина, Очки, Общие очки" << std::endl;

    std::vector<std::string> disciplines = ResultManager::getDisciplines();
    auto results = SQLRequests::getResultsRequest();

    std::map<int, std::vector<Participant>> participantsByYear;
    ResultManager::updateParticipants(participantsByYear, results);

    ResultManager::calculatePoints(participantsByYear, disciplines);
    ResultManager::assignDegrees(participantsByYear);

    for (const auto &[year, participants]: participantsByYear) {
        file << "Год рождения: " << year << std::endl;
        file << "1 степень:" << std::endl;
        saveParticipantResults(file, participants, 1);

        file << "2 степень:" << std::endl;
        saveParticipantResults(file, participants, 2);

        file << "3 степень:" << std::endl;
        saveParticipantResults(file, participants, 3);

        file << std::endl;
    }
}

void CSVManager::saveToCSV(const std::string &filename) {
    std::string _filename = pathFix(filename);
    std::ofstream file(_filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << _filename << std::endl;
        throw std::runtime_error("Failed to open file");
    }

    file << "\xEF\xBB\xBF";

    saveParticipants(file);

    file << std::endl << std::endl;

    saveGroups(file);

    file << std::endl << std::endl;

    saveResults(file);

    file.close();
}

std::string CSVManager::pathFix(const std::string &filename) {
    std::string _filename;
    int quotes = 0;
    for (int i = 0; i < filename.size(); ++i) {
        if (quotes == 0 && filename[i] == '\"') {
            continue;
        }
        _filename += (quotes >= 1 && filename[i] == '\"') ? ' ' : (filename[i] == '\\') ? '/' : filename[i];
        quotes += (filename[i] == '\"') ? 1 : 0;
    }
    return _filename;
}

void CSVManager::saveGroups(std::ofstream &file) {
    int groupCount = getGroupCount();

    for (int groupId = 1; groupId <= groupCount; ++groupId) {
        std::vector<PGresult *> requests = SQLRequests::groupRequest(groupId);
        PGresult *res = requests[0];
        PGresult *_command = requests[1];

        int rows = PQntuples(res);
        file << "Группа номер " << groupId << ":" << std::endl;
        file << "Год рождения: " << PQgetvalue(_command, groupId - 1, 0) << std::endl;

        for (int i = 0; i < rows; i++) {
            file << i + 1 << ". " << PQgetvalue(res, i, 0) << " " << PQgetvalue(res, i, 1) << std::endl;
        }
        file << std::endl;

        PQclear(res);
        PQclear(_command);
    }
}

int CSVManager::getGroupCount() {
    const char *command = "SELECT COUNT(id) FROM groups;";
    PGresult *res = PQexec(Database::conn, command);
    Database::checkError(res);

    if (PQntuples(res) != 1) {
        PQclear(res);
        throw std::runtime_error("Unexpected result from COUNT query");
    }

    int groupCount = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return groupCount;
}
