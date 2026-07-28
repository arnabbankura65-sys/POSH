#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Before fork: my PID is %d\n", getpid());

    pid_t pid = fork();

    if (pid == 0) {
        // CHILD
        printf("I am the CHILD (PID %d), about to exec into 'ls'...\n", getpid());

        char *args[] = {"ls", "-l", NULL};   // argv array, NULL-terminated
        execvp(args[0], args);

        // if execvp succeeds, we NEVER reach this point --
        // the child's memory/code has been replaced by 'ls'
        perror("execvp failed");
        return 1;
    } else if (pid > 0) {
        // PARENT
        int status;
        printf("I am the PARENT (PID %d), waiting for child...\n", getpid());
        waitpid(pid, &status, 0);
        printf("Parent: child (which became 'ls') has finished\n");
    } else {
        perror("fork failed");
    }

    printf("This line runs in BOTH... wait, does it? PID %d\n", getpid());
    return 0;
}
