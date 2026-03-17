# Mini-Shell

A lightweight Unix shell implemented in modern C++17. Supports built-in commands, external program execution via `fork`/`execvp`, and a clean layered architecture designed for extensibility.

> Demo GIF coming soon — recorded with asciinema

---

## Features

**Built-in commands**

| Command | Behavior |
|--------|----------|
| `cd <path>` | Change to the given directory |
| `cd` | Change to home directory |
| `cd -` | Switch to previous directory |
| `pwd` | Print current working directory |
| `exit` | Exit the shell |

**External program execution**

- Runs any program on `$PATH` (e.g. `ls`, `grep`, `cat`)
- Uses `fork()` + `execvp()` + `waitpid()` — correct POSIX model
- Child exits via `_exit()` to avoid double-flushing stdio

**Shell state tracking**

- Tracks `currentDir` and `previousDir` for `cd -` support
- Error reporting via `perror()`
- Directory resolution via `getcwd()`

---

## Project Structure

```
Mini-Shell/
├── include/
│   ├── builtins/
│   │   └── builtins.hpp
│   ├── command/
│   │   └── command.hpp
│   ├── executor/
│   │   └── executor.hpp
│   ├── parser/
│   │   └── parser.hpp
│   ├── shell/
│   │   └── shell.hpp
│   ├── utils/
│   │   └── utils.hpp
│   └── Interface/
│       └── IshellContext.hpp
├── src/
│   ├── builtins/
│   │   └── builtins.cpp
│   ├── command/
│   │   └── command.cpp
│   ├── executor/
│   │   └── executor.cpp
│   ├── parser/
│   │   └── parser.cpp
│   ├── shell/
│   │   └── shell.cpp
│   ├── utils/
│   │   └── utils.cpp
│   └── main.cpp
├── docs/
│   ├── architecture/
│   │   └── architecture.md
│   └── diagrams/
│       ├── shell_sequence_diagram.svg
│       ├── shell_class_diagram.svg
│       ├── parser_class_diagram.svg
│       ├── executor_class_diagram.svg
│       ├── builtins_class_diagram.svg
│       └── command_class_diagram.svg
├── tests/
├── CMakeLists.txt
└── README.md
```

**Component responsibilities**

- `Parser` — tokenizes raw input into a `Command` struct
- `Executor` — dispatches to builtins or forks an external process
- `Builtins` — implements shell-internal commands (`cd`, `pwd`, `exit`)
- `Shell` — owns the REPL loop (read → parse → execute → repeat)
- `IshellContext` — interface for shell state shared across components
- `Utils` — shared helpers

---

## Build

**Requirements**

- C++17 or newer
- CMake ≥ 3.10
- `g++` or `clang++`
- Linux / Unix

**Steps**

```bash
git clone https://github.com/KagE-Akumaa/Mini-Shell.git
cd Mini-Shell
mkdir build && cd build
cmake ..
make
./mini_shell
```

---

## Usage

```bash
mini-shell> pwd
/home/user

mini-shell> ls -la

mini-shell> cd /tmp

mini-shell> cd -          # back to previous directory

mini-shell> exit
```

---

## Roadmap

- [ ] Pipe support (`cmd1 | cmd2`)
- [ ] I/O redirection (`>`, `>>`, `<`)
- [ ] Environment variable expansion (`$VAR`)
- [ ] Command history (up/down arrows)
- [ ] Signal handling (`Ctrl+C`, `Ctrl+Z`)

---

## License

MIT
