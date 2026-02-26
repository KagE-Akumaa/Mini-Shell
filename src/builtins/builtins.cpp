#include "builtins/builtins.hpp"

Builtins::Builtins() {
  table["cd"] = [this](Command &cmd) { run_cd(cmd); };
  table["pwd"] = [this](Command &cmd) { run_pwd(cmd); };
  table["exit"] = [this](Command &cmd) { run_exit(cmd); };
}

void Builtins::run_cd(const Command &cmd) {}

void Builtins::run_exit(const Command &cmd) {}

void Builtins::run_pwd(const Command &cmd) {}

bool Builtins::isBuiltin(const std::string &name) const {
  return table.find(name) != table.end();
}

void Builtins::dispatch(Command &cmd) {
  // get the program name
  auto it = table.find(cmd.getProgram());

  if (it != table.end()) {
    // if its a builtin then dispatch the right function
    it->second(cmd);
  }
}
