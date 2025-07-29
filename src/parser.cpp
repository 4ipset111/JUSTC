#include "parser.h"

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
    if (!match(TokenType::StringLiteral))
    {
        error("Expected string after 'is'");
        return;
    }
    std::string value = previous().value;

    /*if (!match(TokenType::Comma))
    {
        error("Expected ',' after string value");
        return;
    }*/

    variables[name] = value;
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
            std::cout << variables[name] << std::endl; // raw value
        }
        else
        {
            error("Undefined variable: " + name);
        }
    }
    // Case 2: ECHO variable (quoted output)
    else if (match(TokenType::Identifier))
    {
        std::string name = previous().value;
        if (variables.count(name))
        {
            std::cout << "\"" << variables[name] << "\"" << std::endl; // quoted value
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
    else
    {
        error("Expected VALUE(variable), variable name, or string literal after ECHO");
    }
}