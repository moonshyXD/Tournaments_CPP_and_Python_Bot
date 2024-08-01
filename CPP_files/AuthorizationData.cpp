#include "AuthorizationData.h"

const std::map<std::string, std::string> AuthorizationData::authorizationData{
        {"HOST", "localhost"},
        {"PORT", "5432"},
        {"DATABASE_NAME", "postgres"},
        {"USER", "postgres"},
        {"PASSWORD", "bobkb170307"}
};

std::string AuthorizationData::getData(const std::string &messageName) {
    for (const auto &pair : authorizationData) {
        if (messageName == pair.first) {
            std::string message = pair.second;
            return message;
        }
    }

    spdlog::warn("Authorization data not found", messageName);
    return "Сообщение с таким названием не найдено ";
}
