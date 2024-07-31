#include "ErrorMessages.h"

const std::map<std::string, std::string> ErrorMessages::errorMessages {
        {"CONNECTION_FAILED", "Не удалось подключиться к базе данных"},
        {"OPENING_ERROR", "Ошибка при открытии файла: "},
        {"UNKNOWN_OPERATION", "Неизвестная операция: "},
        {"ERROR_EXECUTING_COMMAND", "Ошибка выполнения команды: "},
        {"COMMAND", "Command: "},
        {"CONNECTION_IS_NOT_ESTABLISHED", "Соединение не установлено"},
        {"QUERY_EXECUTING_ERROR", "Ошибка выполнения запроса: "},
        {"DATABASE_ERROR", "Произошла ошибка базы данных"},
        {"TRANSLATE_ERROR", "Ошибка перевода текста"}
};

void ErrorMessages::printError(const std::string &messageName) {
    std::cerr << getMessage(messageName) << std::endl;
}

std::string ErrorMessages::getError(const std::string &messageName) {
    for (const auto &pair : errorMessages) {
        if (messageName == pair.first) {
            std::string message = pair.second;
            return message;
        }
    }

    return "Сообщение с таким названием не найдено ";
}