#include "Operations.h"
#include "Translate.h"

int main(int argc, char* argv[]) {
    try {
        std::string message = "Выполнение команды...";
        std::cout << message << std::endl;

        Operations operation{};
        operation.handleEvents(argc, argv);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

// Массив disciplines ResultManager.cpp -> checkFirst();

// что то через idшки надо как то сделать, чтобы работать через них, а не через ФИО

// .idea gitignore, C-файлы сделать запихнуть в отедльную папку, Python-файлы в другой
// также все текста запихнуть в отдельные файлы (для питона обязательно, для с++ по желанию и sql тоже)
// логирование