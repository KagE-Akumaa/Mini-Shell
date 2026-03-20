#include "executor/executor.hpp"
#include "command/command.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

Executor::Executor(Builtins *builtin) : builtin(builtin) {}

void Executor::execute(Command &cmd) {
    // Check it the cmd is builtin if yes then executeBuiltin else
    // executeExternal

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
        auto &redirection = cmd.getRedirection();

        for (auto &r : redirection) {
            // NOTE: 0644 is the standard permission (owner read/write,
            // group/others read). Without it, the created file gets random
            // permissions since the third argument is unspecified.
            // For "<" the file already exist so no need.
            if (r.operand == "<") {
                // open with O_RDONLY, dup2 to fd 0 - STDIN_FILENO
                int ffd = open(r.fileDesc.c_str(), O_RDONLY);
                if (ffd == -1) {
                    perror("open");
                    _exit(EXIT_FAILURE);
                }

                if (dup2(ffd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    _exit(EXIT_FAILURE);
                }
                close(ffd);
            } else if (r.operand == ">") {
                // open with O_WRONLY | O_CREAT | O_TRUNC, dup2 to fd 1 -
                // STDOUT_FILENO
                int ffd = open(r.fileDesc.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                               0644);
                if (ffd == -1) {
                    perror("open");
                    _exit(EXIT_FAILURE);
                }

                if (dup2(ffd, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    _exit(EXIT_FAILURE);
                }
                close(ffd);
            } else if (r.operand == ">>") {
                // open with O_WRONLY | O_CREAT | O_APPEND, dup2 to fd 1 -
                // STDOUT_FILENO
                int ffd = open(r.fileDesc.c_str(),
                               O_WRONLY | O_APPEND | O_CREAT, 0644);
                if (ffd == -1) {
                    perror("open");
                    _exit(EXIT_FAILURE);
                }

                if (dup2(ffd, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    _exit(EXIT_FAILURE);
                }
                close(ffd);

            } else if (r.operand == "2>") {
                // open with O_WRONLY | O_CREAT | O_TRUNC, dup2 to fd 2 -
                // STDERR_FILENO
                int ffd = open(r.fileDesc.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                               0644);
                if (ffd == -1) {
                    perror("open");
                    _exit(EXIT_FAILURE);
                }
                if (dup2(ffd, STDERR_FILENO) == -1) {
                    perror("dup2");
                    _exit(EXIT_FAILURE);
                }
                close(ffd);
            }
        }
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
