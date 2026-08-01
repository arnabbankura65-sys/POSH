#pragma once
#include <string>
#include <vector>

class Tokenizer{
public:
    //Returns tokens from input line. empty vector for empty input line.
    //std::runtime_error on unterminated qoutes or stray escapes.
    std::vector<std::string> tokenize(const std::string& line) const;

private:
    //Helper function that detects white spaces and skip until a valid non-ws position comes.
    //returns line.size() if string ends.
    size_t skipWhitespace(const std::string& line, size_t pos) const;
    //Reads one starting at pos. Handles qoutes and escapes.
    //Advances pos past the token. Throws on malformed input.
    std::string readToken(const std::string& line,size_t& pos)const;
    char unescape(char c) const;

};
