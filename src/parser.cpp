#include "parser.h"
#include <algorithm>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

void Parser::execute()
{
    bool expectComma = true;
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

        if (isAtEnd()) break;

        if (check(TokenType::Comma))
        {
            advance();
            expectComma = true;
        }
        else if (check(TokenType::Dot))
        {
            advance();
            expectComma = false;
            if (!isAtEnd() && peek().type != TokenType::EndOfFile)
            {
                error("Code after '.' is not allowed");
            }
            break;
        }
        else
        {
            error("Expected ',' after statement, or '.' at the end");
            break;
        }
    }

    if (expectComma)
    {
        error("Code must end with a '.'");
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
    else if (match(TokenType::FloatLiteral))
    {
        variables[name] = normalizeFloat(previous().value);
    }
    else if (match(TokenType::Identifier))
    {
        variables[name] = previous().value;
    }
    else
    {
        error("Expected string, number, or variable name after 'is'");
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

        std::string resolved = resolveValue(name);

        bool isNumeric = true;
        try
        {
            std::stod(resolved);
        }
        catch (...)
        {
            isNumeric = false;
        }

        if (isNumeric)
        {
            std::cout << resolved << std::endl;
        }
        else
        {
            std::cout << "\"" << resolved << "\"" << std::endl;
        }
    }
    // Case 2: ECHO variable
    else if (match(TokenType::Identifier))
    {
        std::string name = previous().value;

        std::string resolved = resolveValue(name);

        bool isNumeric = true;
        try
        {
            std::stod(resolved);
        }
        catch (...)
        {
            isNumeric = false;
        }

        if (isNumeric)
        {
            std::cout << resolved << std::endl;
        }
        else
        {
            std::cout << "\"" << resolved << "\"" << std::endl;
        }
    }
    // Case 3: ECHO "string"
    else if (match(TokenType::StringLiteral))
    {
        std::cout << previous().value << std::endl;
    }
    // Case 4: ECHO number
    else if (match(TokenType::IntLiteral))
    {
        std::cout << previous().value << std::endl;
    }
    else
    {
        error("Expected VALUE(variable), variable name, string or number after ECHO");
    }
}

std::string Parser::resolveValue(const std::string& name)
{
    std::unordered_set<std::string> visited;
    return resolveValue(name, visited);
}

std::string Parser::resolveValue(const std::string& name, std::unordered_set<std::string>& visited)
{
    if (visited.count(name))
    {
        error("Circular reference detected for: " + name);
        return "";
    }

    auto it = variables.find(name);
    if (it == variables.end())
    {
        error("Undefined variable: " + name);
        return "";
    }

    visited.insert(name);
    const std::string& value = it->second;

    // If value is another variable, resolve it further
    if (variables.count(value))
    {
        return resolveValue(value, visited);
    }

    return value;
}
