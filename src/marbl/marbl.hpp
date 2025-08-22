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

    static int runFile(char *filepath);
    static int runPrompt();
};
