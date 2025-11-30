✅ PART 1 – Understand What a Shell Actually Is

A shell is just a program that:

Reads input from the user (a command string).

Parses that string into tokens (command + arguments).

Creates a new process using fork().

Executes the command in the child using exec() family.

Waits for the command to finish (parent uses wait()).

Loops back to read the next command.

That’s it.
Your mini shell will slowly evolve to support more functionalities.

🔥 We will build it in stages

Each stage is small and understandable.

🧩 Stage 1 — Design the Architecture

Think of your shell like a pipeline:

         ┌──────────┐      ┌────────────┐      ┌───────────┐
Input →  │ Readline │ →     │ Parser     │ →    │ Executor  │ → Output
         └──────────┘      └────────────┘      └───────────┘

1. Readline Component

Shows a prompt (msh>)

Reads a full line from STDIN

Should handle EOF (CTRL+D)

2. Parser Component

Split the input into:

Command (e.g., "ls")

Arguments (e.g., "-l", "/home")

Must handle multiple spaces

Should prepare a vector/array of strings for execution

3. Executor Component

Handles built-in commands OR external commands

4. Built-in Command Module

Commands handled inside your program, e.g.:

Command	Description
cd	Change directory
exit	Quit shell
pwd	Print working directory

These do NOT use fork/exec.

5. External Command Execution

For commands like ls, grep, cat

Use:

fork() – create child

execvp() – replace child with actual program

waitpid() – parent waits

🧱 Stage 2 — System Design Approach

Before writing code, think about the architecture like a designer.

🔹 Request Flow
User types → shell parses → decides → runs built-in OR exec → prints result

🔹 Important Design Constraints

Shell must never crash — even on bad input.

Shell must keep running until exit is typed.

Shell must isolate user commands in new processes.

Shell must handle PATH lookup automatically (execvp does this).

🔹 Non-goals for Now

We are NOT implementing:

Piping (|)

Redirection (>, <)

Job control (&)

Environment variables

Script files

We will add them later.

🎯 STAGE 3 — Task Breakdown (DSA Style)

Here are challenges for you.

📝 Task 1 — Build the Prompt + Input System

Goal: Your shell prints a prompt like:

msh>


Then reads a full line.

Key design questions:

How will you detect EOF?

How will you store the input?

What if the user presses Enter without typing anything?

Your shell loop:

loop:
    show prompt
    read command
    parse/execute

📝 Task 2 — Implement the Parser

Input:

ls   -l   /home


Output representation:

command: "ls"
args: ["ls", "-l", "/home", NULL]


Key design questions:

How will you handle repeated spaces?

How to split without destroying original string?

What is the time complexity?

📝 Task 3 — Decide: Built-in or External Command?

If user types:

cd /home


You must NOT use exec.
This is internal.

If user types:

ls -l


You must fork & exec.

Key design question:
How will you detect that a command is built-in?

A map?

If statements?

A function pointer table?

📝 Task 4 — Execute External Commands

Process:

fork → execvp → waitpid


Key design questions:

What happens if execvp fails?

Should the parent wait or not?

How will you print errors (like bash "command not found")?

📝 Task 5 — Add Built-in Commands

Start with 3:

cd

pwd

exit

Key design questions:

cd modifies the process directory — why must this be in the parent?

What will happen if user types: cd with no arguments?

How will you fetch current working directory?


⭐️ 4. HOW TO REWRITE YOUR SHELL WITH OS KNOWLEDGE

Here’s the roadmap using your new understanding:

🟩 Step 1 — Clean up command parsing

remove checkSpaces() complexity

use stringstream or tokenization

parse operators: |, <, >, >>, &

🟩 Step 2 — Improve fork + exec correctness

Use vector<char*> → avoids invalid memory.

🟩 Step 3 — Add background processes

Use:

if (background) skip waitpid()

🟩 Step 4 — Add pipes (multi-pipe chained)

This is a big jump.
Your OS knowledge makes it possible now.

🟩 Step 5 — Add redirection

Use dup2().

🟩 Step 6 — Add environment variable support**
