# 🐚 Mini Shell (C++17)

A lightweight Unix-style shell implemented in modern C++ with support
for built-in commands, external program execution, and proper
POSIX-compliant behavior.\
This shell will later be integrated into a **custom Qt-based terminal
UI**.

## 🚀 Features

### ✔ Built-in Commands

  Command   Description
  --------- ----------------------------------------------
  `cd`      Change directory (`cd`, `cd <path>`, `cd -`)
  `pwd`     Print the current working directory
  `exit`    Exit the shell

### ✔ External Program Execution

-   Supports running standard system programs (`ls`, `cat`, `grep`,
    etc.)
-   Uses correct `fork()` + `execvp()` model
-   Parent process waits using `waitpid()`
-   Safe child process termination using `_exit()`

### ✔ Shell Context Tracking

The shell maintains: - `currentDir` --- current working directory\
- `previousDir` --- directory used for `cd -`

### ✔ Correct POSIX Behavior

-   Argument validation\
-   Proper error reporting with `perror()`\
-   Uses `getcwd()` for reliable directory resolution

### ✔ Extensible Architecture

The project is designed with clean abstractions: - `Parser` ---
tokenizes input into a `Command`\
- `Executor` --- dispatches builtins or external programs\
- `Builtins` --- handles shell-internal commands\
- `Shell` --- main REPL loop\
- `IshellContext` --- stores state shared between components

## 🖥 Integration With Qt (Upcoming)

This shell backend will be integrated into a **Qt-based terminal
emulator**, where: - The C++ shell runs as the backend interpreter\
- Qt provides a GUI terminal view, input handling, output rendering, and
theme customization

## 📦 Project Structure

    .
    ├── include/
    │   ├── builtins/
    │   ├── executor/
    │   ├── parser/
    │   ├── shell/
    │   └── command/
    ├── src/
    │   ├── builtins/
    │   ├── executor/
    │   ├── parser/
    │   ├── shell/
    │   └── command/
    ├── CMakeLists.txt
    └── README.md

## 🔧 Build Instructions

### Requirements

-   C++17 or newer\
-   Linux/Unix environment\
-   CMake (≥ 3.10)\
-   g++ or clang++

### Build

``` bash
git clone https://github.com/<your-user>/<your-repo>.git
cd <your-repo>
mkdir build && cd build
cmake ..
make
```

### Run

``` bash
./mini-shell
```

## ✨ Examples

### Run a builtin:

``` bash
mini-shell> pwd
```

### Run external command:

``` bash
mini-shell> ls -la
```

### Switch to previous directory:

``` bash
mini-shell> cd -
```

### Exit:

``` bash
mini-shell> exit
```

## 📌 Roadmap

-   [ ] Add pipes (`|`)
-   [ ] Add redirection (`>`, `>>`, `<`)
-   [ ] Support environment variables
-   [ ] Command history
-   [ ] Qt terminal integration

## 🤝 Contributing

Contributions are welcome!

## 📄 License

MIT License.
