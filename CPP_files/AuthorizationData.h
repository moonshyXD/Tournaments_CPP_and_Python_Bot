#ifndef TOURNAMENTBOT_AUTHORIZATIONDATA_H
#define TOURNAMENTBOT_AUTHORIZATIONDATA_H

#include "GeneralMessages.h"
#include <map>
#include <string>
#include <spdlog/spdlog.h>
#include <iostream>

class AuthorizationData : public GeneralMessages {
private:
    static const std::map<std::string, std::string> authorizationData;

public:
    static std::string getData(const std::string& messageName);

};


#endif
