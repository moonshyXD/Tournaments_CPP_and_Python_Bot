#include "Messages.h"

std::unordered_map<std::string, std::string> Messages::messages = {
        {"COMMAND_EXECUTING", "Выполнение команды..."},
        {"POST_PARTICIPANT_SUCCESS", "Участник успешно добавлен"},
        {"DELETE_PARTICIPANT_SUCCESS", "Участник успешно удален"},
        {"POST_PARTICIPANT_ERROR", "Этот участник уже был добавлен"},
        {"PARTICIPANT_HAS_NOT_FOUND", "Участник не найден"},
        {"POST_RESULT_SUCCESS", "Результат участника успешно добавлен"},
        {"POST_RESULT_ERROR", "Результат для этого участника в этой дисциплине уже добавлен"},
        {"DELETE_RESULT_SUCCESS", "Результат участника успешно удален"},
        {"RESULT_HAS_NOT_FOUND", "Результата для этого участника в этой дисциплине еще нет"},
        {"GROUP_NUMBER", "Группа номер: "},
        {"PARTICIPANTS", "Участники:"},
        {"FIRST_NAME_LAST_NAME_AGE", "Имя Фамилия Год рождения"},
        {"TOTAL_POINTS", "Общие очки: "},
        {"AGE", "Год рождения: "},
        {"FIRST_DEGREE", "1 степень:"},
        {"SECOND_DEGREE", "2 степень:"},
        {"THIRD_DEGREE", "3 степень:"},
        {"RESULTS", "Результаты:"},
        {"RESULTS_INTERFACE", "Имя, Фамилия, Дисциплина, Очки, Общие очки"},
        {"CONNECTION_FAILED", "Не удалось подключиться к базе данных"},
        {"OPENING_ERROR", "Ошибка при открытии файла: "},
        {"UNKNOWN_OPERATION", "Неизвестная операция: "},
        {"ERROR_EXECUTING_COMMAND", "Ошибка выполнения команды: "},
        {"COMMAND", "Command: "},
        {"CONNECTION_IS_NOT_ESTABLISHED", "Соединение не установлено"},
        {"QUERY_EXECUTING_ERROR", "Ошибка выполнения запроса: "},
        {"DATABASE_ERROR", "Произошла ошибка базы данных"},
        {"TRANSLATE_ERROR", "Ошибка перевода текста"},
        {"HOST", "localhost"},
        {"PORT", "5432"},
        {"DATABASE_NAME", "postgres"},
        {"USER", "postgres"},
        {"PASSWORD", "bobkb170307"}
};

const std::string Messages::unknownMessage = "Сообщение не найдено";

const std::string &Messages::getMessage(const std::string &key) {
    auto it = messages.find(key);
    if (it != messages.end()) {
        return it->second;
    }
    return unknownMessage;
}

void Messages::printMessage(const std::string &key) {
    std::string message = getMessage(key);
    std::cout << message << std::endl;
    spdlog::info(message);
}
