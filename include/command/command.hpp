#pragma once
#include <memory>
#include <string>
#include <vector>

class Command;
struct Redirection {
    std::string operand;
    std::string fileDesc;
};
struct Pipes {
    std::unique_ptr<Command> leftSide;
    std::unique_ptr<Command> rightSide;
};
class Command {
  private:
    std::string program;
    std::vector<std::string> arguments;
    std::vector<Redirection> redirection;
    std::unique_ptr<Pipes> piping;

  public:
    Command(std::string program, std::vector<std::string> arguments,
            std::vector<Redirection> redirection,
            std::unique_ptr<Pipes> piping);
    const std::string &getProgram() const;
    const std::vector<std::string> &getArguments() const;
    const std::vector<Redirection> &getRedirection() const;
    const Pipes *getPipes() const;
};
