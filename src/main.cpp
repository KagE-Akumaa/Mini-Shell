#include "shell/shell.hpp"
#include <memory>

int main() {
  std::unique_ptr<Builtins> builtin = std::make_unique<Builtins>();
  std::unique_ptr<Parser> parser = std::make_unique<Parser>();
  std::unique_ptr<Executor> executor =
      std::make_unique<Executor>(builtin.get());
  std::unique_ptr<Shell> shell =
      std::make_unique<Shell>(parser.get(), executor.get());

  shell->run();

  return 0;
}
