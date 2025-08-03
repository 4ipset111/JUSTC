#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <sstream>
#include "lexer.h"

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);

    void execute();

private:
    const std::vector<Token>& tokens;
    size_t pos;
    std::unordered_map<std::string, std::string> variables;

    // === Token Helpers ===
    const Token& peek() const { return tokens[pos]; }
    const Token& previous() const { return tokens[pos - 1]; }

    bool match(TokenType type)
    {
        if (check(type)) { advance(); return true; }
        return false;
    }

    bool check(TokenType type) const
    {
        return !isAtEnd() && tokens[pos].type == type;
    }

    bool check(TokenType type, const std::string& val) const
    {
        return !isAtEnd() && tokens[pos].type == type && tokens[pos].value == val;
    }

    bool checkNext(TokenType type, const std::string& val) const
    {
        return (pos + 1 < tokens.size()) && tokens[pos + 1].type == type && tokens[pos + 1].value == val;
    }

    bool isAtEnd() const { return tokens[pos].type == TokenType::EndOfFile; }
    const Token& advance() { if (!isAtEnd()) ++pos; return previous(); }

    void error(const std::string& message)
    {
        std::cerr << "[Line " << peek().line << "] Error: " << message << std::endl;
    }

    static std::string normalizeFloat(const std::string& value)
    {
        if (value.size() >= 2 && value[0] == '-' && value[1] == '.')
        {
            return "-0" + value.substr(1);
        }
        if (!value.empty() && value[0] == '.')
        {
            return "0" + value;
        }
        return value;
    }

    // === Statements ===
    void parseAssignment();
    void parseEcho();

    std::string resolveValue(const std::string& name, std::unordered_set<std::string>& visited);
    std::string resolveValue(const std::string& name);
};

#endif // __PARSER_H__
