#include "CLI.h"

CLIInput CLI::parse(const char *command)
{
    CLIInput result;
    std::istringstream iss(command);
    std::string token;

    // First token is the command name
    iss >> result.name;

    // Push all args
    while (iss >> token)
    {
        result.args.push_back(token);
    }

    return result;
};