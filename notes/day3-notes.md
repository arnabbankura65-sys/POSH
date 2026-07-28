# OSTEP Ch 4 — Processes

## What is a process?
- A process is a running program. It's not the same as the program file (the .exe/.out). The program is bytes on disk; the process is the live state — memory, registers, PID, open files, etc.

## fork()
- fork() clones the entire calling process (memory, open file descriptors, everything) into a new, independent process. Both the parent and the child then continue executing from the instruction right after fork(). The only way to tell them apart afterward is the return value: in the parent it returns the child's PID (a positive number), in the child it returns 0. This return value matters because it's the only signal each process has to know which "copy" it is and branch accordingly.

## exec() (specifically execvp, the variant we'll use)
- execvp() replaces the calling process's code, memory, and address space with a new program. It keeps the same PID, but everything else about the process is overwritten. On success it never returns — the old program is simply gone. It only returns if something failed (e.g., command not found). The `v` in execvp means the arguments are passed as a vector/array (like `args[]`) instead of individual comma-separated arguments; the `p` means it searches the PATH environment variable for the command, so we can write "ls" instead of "/bin/ls".

## waitpid()
- The parent calls waitpid() to block until a specific child finishes running, and to collect that child's exit status. Without it, the parent has no way to know when the child is done, and a finished-but-uncollected child becomes a zombie process — done running, but still taking up an entry in the process table until its parent reaps it. The `&status` argument is an output parameter waitpid() fills in with details about how the child exited (normal exit, exit code, killed by a signal, etc.), which we can inspect with macros like WIFEXITED and WEXITSTATUS.

## Why fork+exec is two steps, not one
- Splitting fork() and exec() creates a window — after cloning but before the child gets overwritten by the new program — where the child can modify its own environment: redirecting stdout to a file, connecting to a pipe, changing its working directory, etc. If fork and exec were a single call, there'd be no such window, and none of this per-command setup (which shells like posh rely on for things like `ls > out.txt` or `cat file | grep foo`) would be possible.

## Code snippet I'll reference in Day 5
​```c
pid_t pid = fork();
if (pid == 0) {
    // child
    execvp(args[0], args);
    // only reach here if exec fails
    perror("exec");
    exit(1);
} else if (pid > 0) {
    // parent
    waitpid(pid, &status, 0);
} else {
    // fork failed
    perror("fork");
}
​```

## Open questions
- Why does printf() output from before execvp() sometimes get lost (buffering + exec wiping unflushed buffers)?
- Ordering of parent vs child output is not guaranteed unless explicitly synchronized (e.g. with waitpid()) — worth remembering when debugging Day 5.

# Write a Shell in C — Notes (Sections 1-3)

Source: https://brennan.io/2015/01/16/write-a-shell-in-c/

## Section 1 — What's a shell
- A shell is just a program whose whole job is: read commands typed by the user, run them, repeat.
- Confirms what I already knew from Day 3 discussion — nothing new here.

## Section 2 — The basic loop

His `main()` structure boils down to:

​```
lsh_loop() {
    do {
        print_prompt();
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
    } while (status);
}
​```

- **lsh_read_line()** — reads one line of raw text the user typed (e.g. `ls -l -a`). In his C version this means manually managing a growable character buffer (allocating memory, doubling it if the line is longer than expected, reading character by character). This is the part of C that's genuinely painful — no built-in resizable string type.
  - How this compares to what I'll write: C++ has `std::string` and `std::getline()`, which does all of this buffer-growing for me automatically. I won't need to hand-roll a buffer resize loop like he does.

- **lsh_split_line()** — takes the raw line string and breaks it into an array of individual argument tokens (so `"ls -l -a"` becomes `["ls", "-l", "-a"]`). He does this with `strtok()`, splitting on whitespace, and stores the result in a raw `char**` array he also has to manually resize as needed.
  - How this compares to what I'll write: I'll likely use `std::stringstream` or manual splitting into a `std::vector<std::string>`, which avoids manual pointer/array resizing entirely.

- **lsh_execute()** — first checks if the command is one of a few built-in commands he implements directly inside the shell itself (things like `cd`, `help`, `exit` — these can't just be exec'd as external programs, because e.g. `cd` has to change the shell's own working directory, not a child's). If it's not a built-in, it falls through to `lsh_launch()`.

## Section 3 — lsh_launch() (fork + exec + wait)

This is the function that actually runs external commands (anything that isn't a built-in). Structurally, it's exactly the same pattern I already wrote and ran myself in `scratch-fork-test.c`:

1. `fork()` the current process
2. In the child (`pid == 0`): call `execvp()` with the command + its args array; if that fails, print an error and exit
3. In the parent: call `waitpid()`, looping on `WIFEXITED`/`WIFSIGNALED` of the status until the child has actually finished

He also handles the `fork() < 0` (fork itself failed) case with an error message, same as I did.

**Key realization:** I already have working, tested code for this exact pattern from today's Block 1 exercise (`scratch-fork-test.c`). His `lsh_launch()` is essentially the same logic wrapped as a reusable function that takes an args array and returns a status. I don't need to memorize his C syntax — I need to remember the *shape*: fork → branch → child execs → parent waits → return status.

## What I'd do differently in C++ (Day 5 plan)

- `char*` / `char**` raw pointers → `std::string` and `std::vector<std::string>`
- Manual buffer-doubling in `lsh_read_line` → `std::getline(std::cin, line)`
- Manual token array building in `lsh_split_line` → split into a `std::vector<std::string>`, then convert to `char**` only right before calling `execvp()` (since `execvp` requires a raw `char* const argv[]` — this is one place C++ still has to "drop down" to C-style arrays because it's a C library call)
- His built-in command dispatch (`cd`, `help`, `exit`) via string comparison → I'll likely do the same idea, just with `std::string ==` comparisons instead of `strcmp()`

## Open questions / things to revisit on Day 5
- Exact syntax for converting a `std::vector<std::string>` into the `char* const argv[]` that `execvp()` expects — need to walk through this carefully since it involves pointer lifetimes (the C strings have to stay alive while execvp uses them).
- How built-in commands (like `cd`) should be structured in my C++ version — as a lookup table/map of command name to function, or a simple if/else chain like his?