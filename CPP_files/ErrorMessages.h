#ifndef TOURNAMENTBOT_ERRORMESSAGES_H
#define TOURNAMENTBOT_ERRORMESSAGES_H

#include "GeneralMessages.h"
#include <map>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

class ErrorMessages : public GeneralMessages {
private:
    static const std::map<std::string, std::string> errorMessages;

public:
    static void printError(const std::string& messageName);

    static std::string getError(const std::string& messageName);
};


#endif
