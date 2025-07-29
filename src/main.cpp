#include <iostream>
#include "lexer.h"
#include "parser.h"

int main()
{
    std::string script = R"(
        -- This is a comment
        roblox is "rbx,
        ECHO VALUE(roblox).
    )";

    Lexer lexer(script);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);

    for (const auto& token : tokens)
    {
        std::cout << "[" << token.line << "] " << (int)token.type << ": " << token.value << "\n";
    }

    parser.execute();

    return 0;
}
