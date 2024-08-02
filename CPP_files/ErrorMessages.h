#ifndef TOURNAMENTBOT_ERRORMESSAGES_H
#define TOURNAMENTBOT_ERRORMESSAGES_H

#include "Messages.h"

class ErrorMessages : public Messages {
    ErrorMessages() {
        messages["CONNECTION_FAILED"] = "Не удалось подключиться к базе данных";
        messages["OPENING_ERROR"] = "Ошибка при открытии файла: ";
        messages["UNKNOWN_OPERATION"] = "Неизвестная операция: ";
        messages["ERROR_EXECUTING_COMMAND"] = "Ошибка выполнения команды: ";
        messages["COMMAND"] = "Command: ";
        messages["CONNECTION_IS_NOT_ESTABLISHED"] = "Соединение не установлено";
        messages["QUERY_EXECUTING_ERROR"] = "Ошибка выполнения запроса: ";
        messages["DATABASE_ERROR"] = "Произошла ошибка базы данных";
        messages["TRANSLATE_ERROR"] = "Ошибка перевода текста";
    }
};


#endif
