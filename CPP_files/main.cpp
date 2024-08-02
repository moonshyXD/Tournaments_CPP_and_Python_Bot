#include "Libraries.h"

int main(int argc, char *argv[]) {
    try {
        auto file_logger = spdlog::basic_logger_mt("file_logger", "logs/logfile.txt");
        spdlog::set_default_logger(file_logger);
        spdlog::info("Executing command");

        GeneralMessages::printMessage("COMMAND_EXECUTING");

//        int argc = 9;
//        char* argv[] = {"","","","","","","1","","retrieveGroup"};

        Operations operation{};
        operation.handleEvents(argc, argv);

    } catch (const std::exception &e) {
        spdlog::error("Exception caught: {}", e.what());
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

// Массив disciplines ResultManager.cpp -> checkFirst();