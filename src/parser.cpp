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
        else
        {
            error("Unexpected token: " + peek().value + " (expected assignment or ECHO)");
            advance();
            continue;
        }

        // check . or ,
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
        else
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

    if (!match(TokenType::Keyword) || previous().value != "VALUE")
    {
        error("Expected VALUE after ECHO");
        return;
    }

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
        std::cout << variables[name] << std::endl;
    }
    else
    {
        error("Undefined variable: " + name);
    }
}
