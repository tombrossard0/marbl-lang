#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include <errors.hpp>

class Marbl {
  private:
    inline void error(int line, std::string message) {
        Errors::report(line, "", message);
        hadError = true;
    }

  public:
    inline static bool hadError = false;

    static int run(std::string source) {
        // Scanner scanner(source);
        // std::vec<Token> tokens = scanner.scanTokens();

        // for (Token token : tokens) {
        //      std::cout << token << std::endl;
        // }

        if (hadError) EX_DATAERR;

        return EX_OK;
    }

    static int runFile(char *filepath);
    static int runPrompt();
};
