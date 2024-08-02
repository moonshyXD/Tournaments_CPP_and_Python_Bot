#ifndef TOURNAMENTBOT_MESSAGES_H
#define TOURNAMENTBOT_MESSAGES_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <spdlog/spdlog.h>

class Messages {
public:
    static const std::string &getMessage(const std::string &key);

    static void printMessage(const std::string &key);

protected:
    static std::unordered_map<std::string, std::string> messages;
    static const std::string unknownMessage;
};

#endif // TOURNAMENTBOT_MESSAGES_H
