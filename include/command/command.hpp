#pragma once
#include <string>
#include <vector>

struct Redirection {
    std::string operand;
    std::string fileDesc;
};
class Command {
  private:
    std::string program;
    std::vector<std::string> arguments;
    std::vector<Redirection> redirection;

  public:
    Command(std::string program, std::vector<std::string> arguments,
            std::vector<Redirection> redirection);
    const std::string &getProgram() const;
    const std::vector<std::string> &getArguments() const;
    const std::vector<Redirection> &getRedirection() const;
};
