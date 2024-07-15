#include "Operations.h"
#include "Translate.h"

int main(int argc, char* argv[]) {
    try {
        std::string message = "Выполнение команды...";
        std::cout << message << std::endl;

        Operations operation(argv[8]);
        operation.handleEvents(argc, argv);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

// Vector disciplines for change discipline priority ResultManager.cpp -> checkFirst();
