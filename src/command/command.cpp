#include "command/command.hpp"

Command::Command(std::string program, std::vector<std::string> arguments,
                 std::vector<Redirection> redirection,
                 std::unique_ptr<Pipes> piping)
    : program(std::move(program)), arguments(std::move(arguments)),
      redirection(std::move(redirection)), piping(std::move(piping)) {}

const std::string &Command::getProgram() const { return program; }

const std::vector<std::string> &Command::getArguments() const {
    return arguments;
}

const std::vector<Redirection> &Command::getRedirection() const {
    return redirection;
}

const Pipes *Command::getPipes() const { return piping.get(); }
