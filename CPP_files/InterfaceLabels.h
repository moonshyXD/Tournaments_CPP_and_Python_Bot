#ifndef TOURNAMENTBOT_INTERFACELABELS_H
#define TOURNAMENTBOT_INTERFACELABELS_H

#include "GeneralMessages.h"
#include <map>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

class InterfaceLabels : public GeneralMessages{
private:
    static const std::map<std::string, std::string> interfaceLabels;

public:
    static void printMessage(const std::string& messageName);

    static std::string getMessage(const std::string& messageName);
};


#endif
