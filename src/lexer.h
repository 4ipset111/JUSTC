#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>
#include <vector>
#include <functional>

enum class TokenType
{
    Identifier,
    Keyword,
    StringLiteral,
    IntLiteral,
    Comma,
    LParen,
    RParen,
    Dot,
    EndOfFile,
};

struct Token
{
    TokenType type;
    std::string value;
    int line;
};

class Lexer
{
public:
    Lexer(const std::string& input);

    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos;
    int line;

    char peek() const { return input[pos]; }
    char advance() { if (input[pos] == '\n') ++line; return input[pos++]; }

    void skipWhitespaceAndComments();

    static bool isAlphaNumericOrUnderscore(char c) { return isalnum(c) || c == '_'; }
    static bool isDigit(char c) { return isdigit(c); }

    std::string readWhile(std::function<bool(char)> predicate);

    std::string readString();

    bool isKeyword(const std::string& word) const { return word == "is" || word == "ECHO" || word == "VALUE"; }
};


#endif // __LEXER_H__
