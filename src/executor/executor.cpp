#include "executor/executor.hpp"
#include "command/command.hpp"

Executor::Executor(Builtins *builtin) : builtin(builtin) {}

void Executor::execute(Command &cmd) {
  // Check it the cmd is builtin if yes then executeBuiltin else executeExternal
  builtin->isBuiltin(cmd.getProgram()) ? executeBuiltin(cmd)
                                       : executeExternal(cmd);
}
void Executor::executeBuiltin(Command &cmd) { builtin->dispatch(cmd); }

void Executor::executeExternal(Command &cmd) {}
