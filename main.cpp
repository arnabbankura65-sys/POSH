#include "tokenizer.h"
#include "executor.h"
#include <iostream>
#include <string>

int main() {
    Tokenizer tokenizer;
    std::string line;
    std::cout << "posh> " << std::flush;
    while (std::getline(std::cin, line)) {
        try {
            auto tokens = tokenizer.tokenize(line);
            if(tokens.empty())continue;
            if(tokens[0] == "exit")break;
            Executor executor;
            int result = executor.execute(tokens);
        } catch (const std::exception& e) {
            std::cerr << "posh: parse error: " << e.what() << "\n";
        }
        std::cout << "posh> " << std::flush;
    }
    return 0;
}