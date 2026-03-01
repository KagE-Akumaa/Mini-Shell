#include "shell/shell.hpp"
#include <iostream>
#include <limits.h>
#include <unistd.h>

Shell::Shell() {
  char buff[PATH_MAX];
  auto currRes = getcwd(buff, PATH_MAX);
  currentDir = currRes;
  previousDir = currentDir;
}
void Shell::run() {
  while (true) {
    // need to get the input;
    std::cout << "$ ";
    std::string input = "";
    std::getline(std::cin, input);

    handleInput(input);
  }
}

// input handling
void Shell::handleInput(std::string input) {
  auto cmdOpt = parser->parse(input);

  if (!cmdOpt) {
    std::cout << "Invalid or empty command\n";
    return;
  }
  Command &cmd = *cmdOpt;
  executor->execute(cmd);
}
// Getter and Setter's for shell internals
const std::string &Shell::getCurrentDir() const { return currentDir; }
const std::string &Shell::getPreviousDir() const { return previousDir; }

void Shell::setCurrentDir(const std::string &dir) { currentDir = dir; }
void Shell::setPreviousDir(const std::string &dir) { previousDir = dir; }

// Setter for executor and parser
void Shell::setParser(Parser *p) { parser = p; }
void Shell::setExecutor(Executor *e) { executor = e; }
