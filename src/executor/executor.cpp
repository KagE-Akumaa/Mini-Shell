#include "executor/executor.hpp"
#include "command/command.hpp"
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

Executor::Executor(Builtins *builtin) : builtin(builtin) {}

void Executor::execute(Command &cmd) {
  // Check it the cmd is builtin if yes then executeBuiltin else executeExternal

  if (builtin->isBuiltin(cmd.getProgram())) {
    executeBuiltin(cmd);
  } else {
    executeExternal(cmd);
  }
}
void Executor::executeBuiltin(Command &cmd) { builtin->dispatch(cmd); }

void Executor::executeExternal(Command &cmd) {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return;
  }
  if (pid == 0) {
    // Child will call the execvp and executes the command + arguments
    // for execvp we need to convert the arguments to char* [] and it should
    // end with a NULL and the arr[0] will have the command
    auto &name = cmd.getProgram();
    auto &arg = cmd.getArguments();

    std::vector<char *> argv;
    argv.push_back(const_cast<char *>(name.c_str()));

    for (const auto &s : arg) {
      argv.push_back(const_cast<char *>(s.c_str()));
    }
    argv.push_back(nullptr);

    // only return on failure
    execvp(argv[0], argv.data());
    perror("execvp");
    _exit(EXIT_FAILURE);

  } else {
    waitpid(pid, nullptr, 0);
  }
}
