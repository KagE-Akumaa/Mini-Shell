#pragma once
#include "builtins/builtins.hpp"
#include "command/command.hpp"

class Executor {
private:
  Builtins *builtin;

public:
  Executor(Builtins *builtin);
  void execute(Command &cmd);
  void executeBuiltin(Command &cmd);
  void executeExternal(Command &cmd);
};
