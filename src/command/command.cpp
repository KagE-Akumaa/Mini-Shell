#include "command/command.hpp"

Command::Command(std::string program, std::vector<std::string> arguments,
                 std::vector<Redirection> redirection)
    : program(std::move(program)), arguments(std::move(arguments)),
      redirection(std::move(redirection)) {}

const std::string &Command::getProgram() const { return program; }

const std::vector<std::string> &Command::getArguments() const {
    return arguments;
}

const std::vector<Redirection> &Command::getRedirection() const {
    return redirection;
}
