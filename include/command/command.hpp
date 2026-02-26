#pragma once
#include <string>
#include <vector>

class Command {
private:
  std::string program;
  std::vector<std::string> arguments;

public:
  Command(std::string program, std::vector<std::string> arguments);
  const std::string &getProgram() const;
  const std::vector<std::string> &getArguments() const;
};
