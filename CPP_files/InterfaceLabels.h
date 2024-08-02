#ifndef TOURNAMENTBOT_INTERFACELABELS_H
#define TOURNAMENTBOT_INTERFACELABELS_H

#include "Messages.h"

class InterfaceLabels : public Messages {
    InterfaceLabels() {
        messages["GROUP_NUMBER"] = "Группа номер: ";
        messages["PARTICIPANTS"] = "Участники:";
        messages["FIRST_NAME_LAST_NAME_AGE"] = "Имя      Фамилия     Год рождения";
        messages["TOTAL_POINTS"] = "Общие очки: ";
        messages["AGE"] = "Год рождения: ";
        messages["FIRST_DEGREE"] = "1 степень:";
        messages["SECOND_DEGREE"] = "2 степень:";
        messages["THIRD_DEGREE"] = "3 степень:";
        messages["RESULTS"] = "Результаты:";
        messages["RESULTS_INTERFACE"] = "Имя, Фамилия, Дисциплина, Очки, Общие очки";
    }
};


#endif
