#pragma once
#include "command/command.hpp"
#include <functional>
#include <string>
#include <unordered_map>

class Builtins {
private:
  std::unordered_map<std::string, std::function<void(Command &)>> table;
  void run_cd(const Command &cmd);
  void run_exit(const Command &cmd);
  void run_pwd(const Command &cmd);

public:
  Builtins();
  // Check if command is builtin
  bool isBuiltin(const std::string &name) const;

  // Dispatch the correct handler
  void dispatch(Command &cmd);
};
