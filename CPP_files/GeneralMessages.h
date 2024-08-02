#ifndef TOURNAMENTBOT_GENERALMESSAGES_H
#define TOURNAMENTBOT_GENERALMESSAGES_H

#include "Messages.h"

class GeneralMessages : public Messages {
    GeneralMessages() {
        messages["COMMAND_EXECUTING"] = "Выполнение команды...";
        messages["POST_PARTICIPANT_SUCCESS"] = "Участник успешно добавлен";
        messages["DELETE_PARTICIPANT_SUCCESS"] = "Участник успешно удален";
        messages["POST_PARTICIPANT_ERROR"] = "Этот участник уже был добавлен";
        messages["PARTICIPANT_HAS_NOT_FOUND"] = "Участник не найден";
        messages["POST_RESULT_SUCCESS"] = "Результат участника успешно добавлен";
        messages["POST_RESULT_ERROR"] = "Результат для этого участника в этой дисциплине уже добавлен";
        messages["DELETE_RESULT_SUCCESS"] = "Результат участника успешно удален";
        messages["RESULT_HAS_NOT_FOUND"] = "Результата для этого участника в этой дисциплине еще нет";
    }
};


#endif
