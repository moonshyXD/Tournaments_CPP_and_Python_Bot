#ifndef ADDPARTICIPANTS_PARTICIPANTMANAGER_H
#define ADDPARTICIPANTS_PARTICIPANTMANAGER_H

#include "Database.h"

class ParticipantManager {
private:

public:
    static void
    postParticipant(const std::string &firstName, const std::string &lastName, const std::string &year_of_birth);

    static void
    deleteParticipant(const std::string &firstName, const std::string &lastName, const std::string &year_of_birth);

    static void printParticipants();
};


#endif
