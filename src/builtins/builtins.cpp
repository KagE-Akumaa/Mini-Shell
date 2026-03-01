#include "builtins/builtins.hpp"
#include <iostream>
#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>

Builtins::Builtins(IshellContext &shellContext) : shellContext(shellContext) {
  table["cd"] = [this](Command &cmd) { run_cd(cmd); };
  table["pwd"] = [this](Command &cmd) { run_pwd(cmd); };
  table["exit"] = [this](Command &cmd) { run_exit(cmd); };
}

void Builtins::run_cd(const Command &cmd) {
  auto arguments = cmd.getArguments();
  if (arguments.size() > 1) {
    std::cerr << "cd: too many arguments!\n";
    return;
  }

  std::string path;
  // Case 1 cd -> go to home
  if (arguments.empty()) {
    // cd to home
    // getenv give const *char
    const char *home = getenv("HOME");
    if (!home) {
      std::cerr << "cd: HOME not set\n";
      return;
    }
    path = home;
  }
  // case 2 cd -
  else if (arguments[0] == "-") {
    auto prev = shellContext.getPreviousDir();
    if (prev.empty()) {
      std::cerr << "cd: no previous Directory\n";
      return;
    }
    path = prev;
    std::cout << path << "\n";
  }
  // case 3 cd path
  else {
    path = arguments[0];
  }
  // save the current dir to update the previous dir
  char oldPath[PATH_MAX];
  if (!getcwd(oldPath, PATH_MAX)) {
    perror("getcwd");
    return;
  }

  // change the Directory
  if (chdir(path.c_str()) != 0) { // return 0 if ok
    perror("cd");
    return;
  }
  char newPath[PATH_MAX];
  if (!getcwd(newPath, PATH_MAX)) {
    perror("getcwd");
    return;
  }

  // set the current and previous dir in shellContext
  shellContext.setPreviousDir(oldPath);
  shellContext.setCurrentDir(newPath);
}

void Builtins::run_exit(const Command &cmd) {
  auto arg = cmd.getArguments();
  if (arg.size() > 0) {
    std::cerr << "exit: too many arguments!\n";
    return;
  }
  exit(EXIT_SUCCESS);
}

void Builtins::run_pwd(const Command &cmd) {
  auto arg = cmd.getArguments();
  if (arg.size() > 0) {
    std::cerr << "pwd: too many arguments!\n";
    return;
  }
  char cwd[PATH_MAX];
  if (!getcwd(cwd, PATH_MAX)) {
    perror("getcwd");
    return;
  }
  std::cout << cwd << "\n";
}

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
