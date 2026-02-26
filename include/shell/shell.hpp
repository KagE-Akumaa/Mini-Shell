#pragma once

#include "executor/executor.hpp"
#include "parser/parser.hpp"

class Shell {
private:
  Parser *parser;
  Executor *executor;

public:
  Shell(Parser *parser, Executor *executor);
  void run();
  void handleInput(std::string input);
};
