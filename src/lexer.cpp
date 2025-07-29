#include "lexer.h"
#include <iostream>

Lexer::Lexer(const std::string& input) : input(input), pos(0), line(1) {}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    while (true)
    {
        skipWhitespaceAndComments();

        if (pos >= input.size())
        {
            tokens.push_back({ TokenType::EndOfFile, "", line });
            break;
        }

        char ch = peek();

        if (isalpha(ch))
        {
            std::string word = readWhile(isAlphaNumericOrUnderscore);
            if (isKeyword(word))
            {
                tokens.push_back({ TokenType::Keyword, word, line });
            }
            else
            {
                tokens.push_back({ TokenType::Identifier, word, line });
            }
        }
        else if (ch == '"')
        {
            tokens.push_back({ TokenType::StringLiteral, readString(), line });
        }
        else if (ch == ',')
        {
            advance(); tokens.push_back({ TokenType::Comma, ",", line });
        }
        else if (ch == '(')
        {
            advance(); tokens.push_back({ TokenType::LParen, "(", line });
        }
        else if (ch == ')')
        {
            advance(); tokens.push_back({ TokenType::RParen, ")", line });
        }
        else if (ch == '.')
        {
            advance(); tokens.push_back({ TokenType::Dot, ".", line });
        }
        else
        {
            std::cerr << "Unknown character '" << ch << "' at line " << line << std::endl;
            advance(); // skip it
        }
    }
    return tokens;
}

void Lexer::skipWhitespaceAndComments()
{
    while (pos < input.size())
    {
        if (isspace(peek()))
        {
            advance();
        }
        else if (peek() == '-' && pos + 1 < input.size() && input[pos + 1] == '-')
        {
            while (pos < input.size() && input[pos] != '\n') advance(); // skip comment
        }
        else
        {
            break;
        }
    }
}

std::string Lexer::readWhile(bool (*predicate)(char))
{
    std::string result;
    while (pos < input.size() && predicate(peek())) result += advance();
    return result;
}

std::string Lexer::readString()
{
    std::string result;
    advance(); // skip "
    while (pos < input.size() && peek() != '"') result += advance();
    if (peek() == '"') advance(); else std::cerr << "Unterminated string\n";
    return result;
}