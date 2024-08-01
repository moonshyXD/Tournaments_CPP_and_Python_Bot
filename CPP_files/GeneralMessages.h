#ifndef TOURNAMENTBOT_GENERALMESSAGES_H
#define TOURNAMENTBOT_GENERALMESSAGES_H

#include <map>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

class GeneralMessages {
private:
    static const std::map<std::string, std::string> generalMessages;

public:
    static void printMessage(const std::string& messageName);

    static std::string getMessage(const std::string& messageName);
};


#endif
