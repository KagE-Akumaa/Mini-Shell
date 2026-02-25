#include "command/command.hpp"

Command::Command(std::string program, std::vector<std::string> arguments)
    : program(std::move(program)), arguments(std::move(arguments)) {}
