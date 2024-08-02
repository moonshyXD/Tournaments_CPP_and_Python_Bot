#ifndef TOURNAMENTBOT_PARTICIPANT_H
#define TOURNAMENTBOT_PARTICIPANT_H

#include <map>
#include <string>

struct Participant {
    std::string firstName;
    std::string lastName;
    std::map<std::string, int> disciplinePoints;
    int totalPoints;
    int degree;
};

#endif
