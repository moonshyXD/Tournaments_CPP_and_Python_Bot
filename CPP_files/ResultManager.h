#ifndef ADDPARTICIPANTS_RESULTMANAGER_H
#define ADDPARTICIPANTS_RESULTMANAGER_H

#include <map>
#include <algorithm>
#include <vector>
#include <iostream>
#include <spdlog/spdlog.h>
#include "Participant.h"
#include "SQLRequests.h"
#include "InterfaceLabels.h"
#include "GeneralMessages.h"

class ResultManager {
private:
    static bool checkFirst(const std::string &discipline);

    static void printParticipantResults(const std::vector<Participant> &participants,
                                        int degreeNumber);

    static void calculatePoints(std::map<int, std::vector<Participant>> &participantsByYear,
                                const std::vector<std::string> &disciplines);

    static void assignDegrees(std::map<int, std::vector<Participant>> &participantsByYear);

    static void printResults(const std::map<int, std::vector<Participant>> &participantsByYear);

    static void sortPoints(std::vector<Participant> &participants, const std::string &discipline);

    static void assignPoints(std::vector<Participant> &participants, const std::string &discipline);

    static void updateParticipants(std::map<int, std::vector<Participant>> &participantsByYear, PGresult *res);

    static std::vector<std::string> getDisciplines();

    friend class CSVManager;

public:
    static void postResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                           const std::string &year_of_birth, int result);

    static void deleteResult(const std::string &discipline, const std::string &firstName, const std::string &lastName,
                             const std::string &year_of_birth);

    static void determineWinner();
};


#endif
