#include<cctype>
#include<vector>
#include<string>
#include<stdexcept>
#include "tokenizer.h"

size_t Tokenizer::skipWhitespace(const std::string& line,std::size_t pos)const{
    
    while(pos < line.size() && isspace(static_cast<unsigned char>(line[pos]))){
        pos++;
    }
    return pos;
}

std::string Tokenizer::readToken(const std::string& line,size_t& pos)const{
    std::string token;
    while(pos < line.size() && !isspace(line[pos])){
        if(line[pos] == '"'){
            pos++;
            while(pos < line.size() && line[pos] != '"'){
                if(line[pos] == '\\' && pos + 1 < line.size()){
                    pos++;
                    token += line[pos];
                    pos++;
                }
                else{
                    token += line[pos];
                    pos++;
                }
            }
            if(pos == line.size()){
                throw std::runtime_error("Ending quote absent!");
            }
            pos++;
        }
        else if(line[pos] == '\\' && pos + 1 < line.size()){
            pos++;
            token += line[pos];
            pos++;
        }
        else{
            token += line[pos];
            pos++;
        }
    }
    return token;
}

std::vector<std::string> Tokenizer::tokenize(const std::string& line)const{
    size_t pos = 0;
    std::vector<std::string> tokens;
    pos = skipWhitespace(line,0);
    while(pos < line.size()){
        tokens.emplace_back(readToken(line,pos));
        pos = skipWhitespace(line,pos);
    }
    return tokens;
}