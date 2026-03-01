#include "shell/shell.hpp"
#include <unistd.h>

int main() {
  const char *home = getenv("HOME");
  if (home) {
    chdir(home);
  }
  Shell shell;
  Builtins builtins(shell);
  Executor executor(&builtins);
  Parser parser;

  shell.setParser(&parser);
  shell.setExecutor(&executor);

  shell.run();
  return 0;
}
