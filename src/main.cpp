#include <iostream>
#include "lexer.h"
#include "parser.h"

int main()
{
    std::string script = R"(
        testnumber is 1,
        roblox is "rbx",
        ECHO VALUE(testnumber),
        ECHO testnumber,
        ECHO VALUE(roblox),
        ECHO roblox,
        ECHO 42,
        ECHO "test".
    )";

    Lexer lexer(script);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);

    /*for (const auto& token : tokens)
    {
        std::cout << "[" << token.line << "] " << (int)token.type << ": " << token.value << "\n";
    }*/

    parser.execute();

    return 0;
}
