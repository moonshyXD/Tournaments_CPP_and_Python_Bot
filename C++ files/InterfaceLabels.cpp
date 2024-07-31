#include "InterfaceLabels.h"

const std::map<std::string, std::string> InterfaceLabels::interfaceLabels {
        {"GROUP_NUMBER", "Группа номер "},
        {"PARTICIPANTS", "Участники:"},
        {"FIRST_NAME_LAST_NAME_AGE", "Имя      Фамилия     Год рождения"},
        {"TOTAL_POINTS", "Общие очки: "},
        {"AGE", "Год рождения: "},
        {"FIRST_DEGREE", "1 степень:"},
        {"SECOND_DEGREE", "2 степень:"},
        {"THIRD_DEGREE", "3 степень:"},
        {"RESULTS", "Результаты:"},
        {"RESULTS_INTERFACE", "Имя, Фамилия, Дисциплина, Очки, Общие очки"},
        {"SECOND_DEGREE", "2 степень:"},
        {"THIRD_DEGREE", "3 степень:"},
};

void InterfaceLabels::printMessage(const std::string &messageName) {
    std::cout << getMessage(messageName) << std::endl;
}

std::string InterfaceLabels::getMessage(const std::string &messageName) {
    for (const auto &pair : interfaceLabels) {
        if (messageName == pair.first) {
            std::string message = pair.second;
            return message;
        }
    }

    return "Сообщение с таким названием не найдено ";
}

