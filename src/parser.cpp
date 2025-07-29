#include "parser.h"
#include <algorithm>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

void Parser::execute()
{
    while (!isAtEnd())
    {
        if (check(TokenType::Identifier) && checkNext(TokenType::Keyword, "is"))
        {
            parseAssignment();
        }
        else if (check(TokenType::Keyword, "ECHO"))
        {
            parseEcho();
        }
        else if (!isAtEnd())
        {
            error("Unexpected token: " + peek().value);
            advance();
            continue;
        }

        // Handle statement terminators
        if (isAtEnd()) break;

        if (check(TokenType::Comma))
        {
            advance();
        }
        else if (check(TokenType::Dot))
        {
            advance();
            if (!isAtEnd())
            {
                error("Code after '.' is not allowed");
            }
            break;
        }
        else if (!isAtEnd())
        {
            error("Expected ',' or '.' after statement");
            break;
        }
    }
}

void Parser::parseAssignment()
{
    std::string name = advance().value;
    advance(); // skip 'is'

    if (match(TokenType::StringLiteral))
    {
        variables[name] = previous().value;
    }
    else if (match(TokenType::IntLiteral))
    {
        variables[name] = previous().value;
    }
    else
    {
        error("Expected string or number after 'is'");
    }
}

void Parser::parseEcho()
{
    advance(); // skip 'ECHO'

    // Case 1: ECHO VALUE(variable)
    if (check(TokenType::Keyword, "VALUE"))
    {
        advance(); // skip 'VALUE'

        if (!match(TokenType::LParen))
        {
            error("Expected '(' after VALUE");
            return;
        }

        if (!match(TokenType::Identifier))
        {
            error("Expected identifier in VALUE()");
            return;
        }

        std::string name = previous().value;

        if (!match(TokenType::RParen))
        {
            error("Expected ')' after identifier");
            return;
        }

        if (variables.count(name))
        {
            // Raw value output
            std::cout << variables[name] << std::endl;
        }
        else
        {
            error("Undefined variable: " + name);
        }
    }
    // Case 2: ECHO variable (formatted output)
    else if (match(TokenType::Identifier))
    {
        std::string name = previous().value;
        if (variables.count(name))
        {
            const std::string& value = variables[name];
            // Check if value is numeric (all digits)
            bool isNumeric = !value.empty() &&
                std::all_of(value.begin(), value.end(),
                    [](char c) { return std::isdigit(c); });

            if (isNumeric)
            {
                std::cout << value << std::endl;
            }
            else
            {
                std::cout << "\"" << value << "\"" << std::endl;
            }
        }
        else
        {
            error("Undefined variable: " + name);
        }
    }
    // Case 3: ECHO "string literal"
    else if (match(TokenType::StringLiteral))
    {
        std::cout << previous().value << std::endl;
    }
    // Case 4: ECHO integer literal
    else if (match(TokenType::IntLiteral))
    {
        std::cout << previous().value << std::endl;
    }
    else
    {
        error("Expected VALUE(variable), variable name, string or number after ECHO");
    }
}
