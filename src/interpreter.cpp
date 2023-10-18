/*
 * src/interpreter.cpp
 * Wopslang Interpreter Sourcefile
 *
 * 2023, Wops Team
 *
 * */

#include <fstream>
#include <ctime>
#include "runtime/AST.h"
#include "parser/parse.h"

int main(int argc, char **argv) {
    std::cout << "\e[31m" << "Warning: This is alpha version. Some critical issues might be appeared." << "\e[m\n";
    if (argc == 2 || (argc == 3 && String(argv[2]) == "debug") || (argc == 3 && String(argv[2]) == "check")) {
        std::ifstream handler(String(argv[1]).data());
        if (!handler.is_open())
            ErrHandler().CallErr(-1, INTERPRETER_CANNOT_OPEN_FILE, {});

        String codeline;
        std::vector<String> code;

        while (std::getline(handler, codeline)) {
            code.push_back(codeline);
        }

        handler.close();
        
        // interpret
        std::vector<Storage> stor;
        std::vector<String> Token_storage;
        std::vector<std::vector<String>> token_table;

        for (String s: code) token_table.push_back(GetTokenTable(s));
        stor.push_back(Storage());
        AST main(Main, {}, {}, 0);

        if (argc == 3 && String(argv[2]) == "debug") {
            std::clock_t end, start = clock();
            Parse(main, token_table, 0, 0, Token_storage);
            main.Execute(stor);
            end = clock();
            std::cout << "\n=== DEBUG ===\n\e[32m" << "Running Time(ms): " << (double)(end-start) * 1000 / CLOCKS_PER_SEC << "\e[m\n";
            return 0;
        }
        if (argc == 3 && String(argv[2]) == "check") {
            Parse(main, token_table, 0, 0, Token_storage);
            return 0;
        }
        Parse(main, token_table, 0, 0, Token_storage);
        main.Execute(stor);
    } else {
        std::cout << "\e[93m" << "Incorrect command structure. Use command 'Wopslang <filepath>'" << "\e[m\n";
    }
}
