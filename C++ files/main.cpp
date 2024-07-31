#include "Libraries.h"

int main(int argc, char *argv[]) {
    try {
        GeneralMessages::printMessage("COMMAND_EXECUTING");

//        int argc = 9;
//        char* argv[] = {"","","","","","","","C:\\Users\\nadez\\Desktop\\tests\\test.txt","loadParticipantsFromFile"};

        Operations operation{};
        operation.handleEvents(argc, argv);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

// Массив disciplines ResultManager.cpp -> checkFirst();

// логирование (в питоне есть, в плюсах нет)