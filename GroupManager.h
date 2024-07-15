#ifndef ADDPARTICIPANTS_GROUPMANAGER_H
#define ADDPARTICIPANTS_GROUPMANAGER_H

#include "Database.h"
#include <vector>

class GroupManager {
private:
    static std::vector<std::string> getUniqueYearsOfBirth();

    static int createGroup(const std::string &year_of_birth);

    static std::vector<int> getParticipantsByYear(const std::string &year_of_birth);

    static void addParticipantsToGroup(const std::vector<int> &participants, int groupId);

    static void printGroup(PGresult *_command, PGresult *res, int groupId);

public:
    static void getGroup(int groupId);

    static void sortParticipantsIntoGroups();
};


#endif
