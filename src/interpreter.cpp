/*
 * src/interpreter.cpp
 * Wopslang Interpreter Sourcefile
 *
 * 2021, Wops Team
 *
 * */

#include <fstream>
#include "runtime/AST.h"
#include "parser/parse.h"

int main(int argc, char **argv) {
    std::cout << "\e[31m" << "Warning: This is alpha version. Some critical issues might be appeared." << "\e[m\n";
    if (argc == 2) {
        std::ifstream handler(String(argv[1]).data());
        if (!handler.is_open())
            ErrHandler().CallErr("From Interpreter: cannot open the file");

        String codeline;
        std::vector<String> code;

        while (std::getline(handler, codeline)) {
            code.push_back(codeline);
        }

        handler.close();
        
        // interpret
        std::unordered_map<String, Variable> stor;
        AST main(Main, {}, {});
        Parse(main, code);
        main.Execute(stor);
    } else {
        std::cout << "\e[93m" << "Incorrect command structure. Use command 'Wopslang <filepath>'" << "\e[m\n";
    }
}
