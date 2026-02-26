#include "command/command.hpp"

Command::Command(std::string program, std::vector<std::string> arguments)
    : program(std::move(program)), arguments(std::move(arguments)) {}

const std::string &Command::getProgram() const { return program; }

const std::vector<std::string> &Command::getArguments() const {
  return arguments;
}
