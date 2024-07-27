#ifndef ADDPARTICIPANTS_TRANSLATE_H
#define ADDPARTICIPANTS_TRANSLATE_H

#include <iostream>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <locale>
#include <codecvt>

class Translate {
private:
    static int findInEng(char letter);

    static std::string argumentToRussian(const char *arg);

    static bool findNumber(const std::vector<int> &bannedArguments, int number);

public:
    static std::vector<std::string> translateArguments(int argc, char *argv[], const std::vector<int> &bannedArguments);

};


#endif
