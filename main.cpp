#include "tokenizer.h"
#include <iostream>
#include <string>

int main() {
    Tokenizer tokenizer;
    std::string line;
    std::cout << "posh> " << std::flush;
    while (std::getline(std::cin, line)) {
        try {
            auto tokens = tokenizer.tokenize(line);
            for (const auto& tok : tokens) {
                std::cout << "[" << tok << "] ";
            }
            if (!tokens.empty()) std::cout << "\n";
        } catch (const std::exception& e) {
            std::cerr << "posh: parse error: " << e.what() << "\n";
        }
        std::cout << "posh> " << std::flush;
    }
    return 0;
}