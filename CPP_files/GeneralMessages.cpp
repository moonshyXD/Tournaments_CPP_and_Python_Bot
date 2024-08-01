#include "GeneralMessages.h"

const std::map<std::string, std::string> GeneralMessages::generalMessages{
        {"COMMAND_EXECUTING", "Выполнение команды..."},
        {"POST_PARTICIPANT_SUCCESS", "Участник успешно добавлен"},
        {"DELETE_PARTICIPANT_SUCCESS", "Участник успешно удален"},
        {"POST_PARTICIPANT_ERROR", "Этот участник уже был добавлен"},
        {"PARTICIPANT_HAS_NOT_FOUND", "Участник не найден"},
        {"POST_RESULT_SUCCESS", "Результат участника успешно добавлен"},
        {"POST_RESULT_ERROR", "Результат для этого участника в этой дисциплине уже добавлен"},
        {"DELETE_RESULT_SUCCESS", "Результат участника успешно удален"},
        {"RESULT_HAS_NOT_FOUND", "Результата для этого участника в этой дисциплине еще нет"},
};

void GeneralMessages::printMessage(const std::string &messageName) {
    std::string message = getMessage(messageName);
    std::cout << message << std::endl;
    spdlog::info(message);
}

std::string GeneralMessages::getMessage(const std::string &messageName) {
    for (const auto &pair : generalMessages) {
        if (messageName == pair.first) {
            std::string message = pair.second;
            spdlog::info("Got message ", messageName);
            return message;
        }
    }

    std::string notFoundMessage = "Сообщение с таким названием не найдено ";
    spdlog::warn(notFoundMessage);
    return notFoundMessage;
}
