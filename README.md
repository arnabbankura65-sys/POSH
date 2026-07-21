# posh

A Unix shell implemented in C++. Learning project for OS internals (processes, fork/exec, pipes, signals).

## Status

🚧 **In progress** — vacation build (July-August 2026)

Current version: v0.1 (basic command execution via fork+exec)

## Goals

- Tokenize input and execute simple commands (`ls`, `echo`, `pwd`, etc.)
- Implement core builtins (`cd`, `pwd`, `exit`, `history`)
- OOP architecture: `Shell`, `Parser`, `Command`, `Executor` classes
- I/O pipes: `cmd1 | cmd2 | cmd3`
- I/O redirects: `>`, `>>`, `<`
- (Semester) Signals: handle `Ctrl+C`, `Ctrl+Z` correctly
- (Semester) Job control: `jobs`, `fg`, `bg`

## Non-goals (for now)

- Shell scripting language
- Aliases
- Tab completion
- Globbing/expansion
- Job control beyond basics

## Build

Requirements:
- WSL (Ubuntu 22.04+) or any Linux
- `g++` with C++17 support
- `make` and `cmake`

```bash
make        # builds the posh binary
make test   # runs tests
./posh      # starts the shell
```

## Usage
$ ./posh
posh> ls
README.md  Makefile  src/  tests/
posh> echo hello world
hello world
posh> pwd
/home/you/dev/posh
posh> cd /tmp
posh> pwd
/tmp
posh> exit

## Architecture

- `src/main.cpp` — entry point, creates Shell and runs REPL
- `src/shell.{h,cpp}` — REPL loop, owns state (cwd, history)
- `src/parser.{h,cpp}` — tokenize input, build Command objects
- `src/command.h` — Command data class (argv, type, redirects)
- `src/executor.{h,cpp}` — fork+exec+wait, dispatch to builtins or external
- `src/builtins.{h,cpp}` — builtin command registry and implementations
- `src/tokenizer.{h,cpp}` — low-level tokenization
- `tests/` — unit + integration tests

## License

MIT
