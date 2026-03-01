#pragma once

#include "Interface/IshellContext.hpp"
#include "executor/executor.hpp"
#include "parser/parser.hpp"

class Shell : public IshellContext {
private:
  Parser *parser = nullptr;
  Executor *executor = nullptr;
  std::string currentDir;
  std::string previousDir;

public:
  Shell();
  void run();
  const std::string &getCurrentDir() const override;
  const std::string &getPreviousDir() const override;
  void setCurrentDir(const std::string &dir) override;
  void setPreviousDir(const std::string &dir) override;
  void handleInput(std::string input);
  void setParser(Parser *p);
  void setExecutor(Executor *e);
};
