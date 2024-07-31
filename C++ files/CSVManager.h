#ifndef ADDPARTICIPANTS_CSVMANAGER_H
#define ADDPARTICIPANTS_CSVMANAGER_H

#include "Participant.h"
#include <fstream>
#include <vector>
#include <libpq-fe.h>
#include "InterfaceLabels.h"
#include "SQLCommands.h"
#include "ResultManager.h"
#include "SQLRequests.h"

class CSVManager {
private:
    static std::string escapeQuotes(const char *str);

    static std::string pathFix(const std::string &filename);

    static void writeHeaders(std::ofstream &file, const std::vector<std::string> &headers);

    static void writeRows(std::ofstream &file, PGresult *res);

    static void saveParticipants(std::ofstream &file);

    static void saveResults(std::ofstream &file);

    static void saveGroups(std::ofstream &file);

    static int getGroupCount();

    static void saveParticipantResults(std::ofstream &file, const std::vector<Participant> &participants, int degreeNumber);

public:
    static void saveToCSV(const std::string &filename);
};

#endif
