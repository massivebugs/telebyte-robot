#ifndef INTERFACES_CLI
#define INTERFACES_CLI

#include <sstream>
#include <string>
#include <vector>

/**
 * @file CLI.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief Parse command strings
 */

/**
 * @brief Represents input parsed from a command line interface (CLI).
 *
 * This structure holds the name of the command and a vector of arguments.
 */
struct CLIInput
{
    std::string name;
    std::vector<std::string> args;
};

/**
 * @file CLI.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief Provides functionality to parse commands.
 *
 * This class parses a command string into a CLIInput structure, extracting the command name
 * and any additional arguments.
 */
class CLI
{
public:
    /**
     * @brief Parses a command string into a CLIInput structure.
     *
     * @param command The command to parse.
     * @return CLIInput The parsed command and arguments.
     */
    CLIInput parse(const char *command);
};

#endif