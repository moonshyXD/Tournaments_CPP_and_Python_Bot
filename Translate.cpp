#include "Translate.h"

int Translate::findInEng(char letter) {
    std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    int index = -1;
    for (int j = 0; j < alphabet.size(); ++j) {
        if (letter == alphabet[j]) {
            index = j;
            return index;
        }
    }

    std::cerr << index << "     " << letter << std::endl;
    throw std::runtime_error("Ошибка: findInEng()");
}

std::string Translate::argumentToRussian(const char *arg) {
    std::string _arg(arg);
    std::wstring alphabetRus = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    std::wstring w_arg = converter.from_bytes(_arg);
    for (int i = 0; i < w_arg.size(); ++i) {
        int index = findInEng(_arg[i]);
        w_arg[i] = alphabetRus[index];
    }

    std::string result = converter.to_bytes(w_arg);
    return result;
}

bool Translate::findNumber(const std::vector<int> &bannedArguments, int number) {
    for (int i = 0; i < bannedArguments.size(); ++i) {
        if (number == bannedArguments[i]) {
            return true;
        }
    }
    return false;
}

std::vector<std::string>
Translate::translateArguments(int argc, char *argv[], const std::vector<int> &bannedArguments) {
    std::vector<std::string> _argv(argc);

    for (int i = 0; i < argc; ++i) {
        if (!findNumber(bannedArguments, i)) {
            std::string arg = argumentToRussian(argv[i]);
            _argv[i] = arg;
        } else {
            _argv[i] = argv[i];
        }
    }

    return _argv;
}

