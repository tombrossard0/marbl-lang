#pragma once

class Marbl {
  private:
    inline void error(int line, std::string message) {
        Errors::report(line, "", message);
        hadError = true;
    }

  public:
    static bool hadError;

    static int run(std::string source) {
        // Scanner scanner(source);
        // std::vec<Token> tokens = scanner.scanTokens();

        // for (Token token : tokens) {
        //      std::cout << token << std::endl;
        // }

        if (hadError) EX_DATAERR;

        return EX_OK;
    }

    static int runFile(char *filepath) {
        std::ifstream inputFile(filepath);

        if (!inputFile) {
            std::cerr << "Cannot open input file!" << std::endl;
            return EX_NOINPUT;
        }

        std::string line;
        while (std::getline(inputFile, line) && !hadError) {
            std::cout << line << std::endl;
            run(line);
        }

        inputFile.close();

        return EX_OK;
    }

    static int runPrompt() {
        std::string line;

        bool isRunning;
        do {
            std::cout << "> ";
            isRunning = static_cast<bool>(std::getline(std::cin, line));
            std::cout << line << std::endl;

            run(line);
            hadError = false;
        } while (line != "exit" && isRunning);

        return EX_OK;
    }
};
