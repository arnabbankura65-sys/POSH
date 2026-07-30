#pragma once
#include <string>
#include <vector>

class Tokenizer {
public:
    // Returns tokens from input line. Empty input returns empty vector.
    // Throws std::runtime_error on unterminated quotes or stray escapes.
    std::vector<std::string> tokenize(const std::string& line) const;

private:
    // Helper that skips whitespace and returns position of next non-ws char.
    // Returns line.size() if at end.
    size_t skipWhitespace(const std::string& line, size_t pos) const;

    // Reads one token starting at pos. Handles quotes and escapes.
    // Advances pos past the token. Throws on malformed input.
    std::string readToken(const std::string& line, size_t& pos) const;
};