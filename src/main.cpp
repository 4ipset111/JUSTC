#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <regex>
#include <algorithm>

using namespace std;

string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

void runScript(const string& script)
{
    unordered_map<string, string> variables;

    istringstream stream(script);
    string line;

    regex assignRegex("^(\\w+)\\s+is\\s+\\\"([^\\\"]+)\\\",$");
    regex echoRegex("^ECHO\\s+VALUE\\((\\w+)\\)$");

    while (getline(stream, line))
    {
        line = trim(line);

        // skip empty lines and comments
        if (line.empty() || line.rfind("--", 0) == 0)
            continue;

        smatch match;

        // variable
        if (regex_match(line, match, assignRegex))
        {
            string varName = match[1];
            string value = match[2];
            variables[varName] = value;
        }
        // ECHO VALUE
        else if (regex_match(line, match, echoRegex))
        {
            string varName = match[1];
            if (variables.count(varName))
            {
                cout << variables[varName] << endl;
            }
            else
            {
                cerr << "Error: variable '" << varName << "' not defined." << endl;
            }
        }
        else
        {
            cerr << "Unknown line: " << line << endl;
        }
    }
}

int main()
{
    string script = R"(
    -- This is a comment
    roblox is "rbx",
    -- We print the value
    ECHO VALUE(roblox)
    -- Done
    )";

    runScript(script);
    return 0;
}
