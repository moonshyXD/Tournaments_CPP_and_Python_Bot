#include "Libraries.h"

int main(int argc, char *argv[]) {
    try {
        GeneralMessages::printMessage("COMMAND_EXECUTING");

        Operations operation{};
        operation.handleEvents(argc, argv);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

// Массив disciplines ResultManager.cpp -> checkFirst();
