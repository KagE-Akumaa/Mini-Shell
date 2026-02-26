#include "shell/shell.hpp"
#include <iostream>

Shell::Shell(Parser *parser, Executor *executor)
    : parser(parser), executor(executor) {}

void Shell::run() {
  while (true) {
    // need to get the input;
    std::cout << "SShell ";
    std::string input = "";
    std::getline(std::cin, input);

    handleInput(input);
  }
}

void Shell::handleInput(std::string input) {
  auto cmdOpt = parser->parse(input);

  if (!cmdOpt) {
    std::cout << "Invalid or empty command\n";
    return;
  }
  Command &cmd = *cmdOpt;
  executor->execute(cmd);
}
