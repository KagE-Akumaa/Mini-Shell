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
    } else if (cmd.getPipes() != nullptr) {
        executePipes(cmd);
    } else {
        executeExternal(cmd);
    }
}
void Executor::executeBuiltin(Command &cmd) { builtin->dispatch(cmd); }
void Executor::executePipes(Command &cmd) {
    // NOTE: pipe syscall wants a c style array i.e a pointer to array
    std::array<int, 2> fd;
    // NOTE:: pipefd(in this case fd) pipefd[0] will have the read end and
    // pipefd[1] will have the write end
    if (pipe(fd.data()) == -1) {
        perror("pipe");
        _exit(EXIT_FAILURE);
    }

    auto pipes = cmd.getPipes();
    // Fork left child means left side gets executed and put the output to fd[1]
    // instead of STDOUT_FILENO 1
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return;
    }
    if (pid1 == 0) {
        auto &left = pipes->leftSide;
        auto &name = left->getProgram();
        auto &arguments = left->getArguments();
        auto &redirection = left->getRedirection();
        // NOTE: Redirect stdout to pipe write end — left child's output flows
        // into the pipe
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        close(fd[1]);
        // NOTE: No need for read end close it if not closed the right child
        // could potentially never get EOF because the read end is still open
        // somewhere.
        // Rule to remember: - Every process should close every pipe end it
        // doesn't use, immediately after forking.
        close(fd[0]);

        // NOTE: Need to think about this
        if (builtin->isBuiltin(name)) {
            executeBuiltin(*left);

            _exit(EXIT_SUCCESS);
        }

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

        for (const auto &s : arguments) {
            argv.push_back(const_cast<char *>(s.c_str()));
        }
        argv.push_back(nullptr);

        // only return on failure
        execvp(argv[0], argv.data());
        perror("execvp");
        _exit(EXIT_FAILURE);
    }
    // Now for right child means right side gets exceuted and if will read from
    // the fd[0] instead of STDIN_FILENO 0
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return;
    }

    if (pid2 == 0) {
        auto &right = pipes->rightSide;
        auto &name = right->getProgram();
        auto &arguments = right->getArguments();
        auto &redirection = right->getRedirection();

        // NOTE: Redirect stdin to pipe read end — right child reads from pipe
        // instead of terminal
        if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        close(fd[0]);
        // NOTE: Left child owns the write end — keeping it open here would
        // prevent
        // the right child from ever seeing EOF on its read end.
        close(fd[1]);

        if (builtin->isBuiltin(name)) {
            executeBuiltin(*right);
            _exit(EXIT_SUCCESS);
        }
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

        for (const auto &s : arguments) {
            argv.push_back(const_cast<char *>(s.c_str()));
        }
        argv.push_back(nullptr);

        // only return on failure
        execvp(argv[0], argv.data());
        perror("execvp");
        _exit(EXIT_FAILURE);
    }
    // Parent need to close fd[2]
    // NOTE: why to close both - kernel has a record of each read and write if
    // we dont close it , it will cause the system to hang because if read is
    // open kernel expects that some program will read from it same for write
    // and could not get to the eof
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}
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
