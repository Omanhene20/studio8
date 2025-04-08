// call_sleeper.c - Modified with SIGINT handler

#include <unistd.h>     // fork(), execvp(), waitpid()
#include <stdlib.h>     // exit()
#include <stdio.h>      // printf()
#include <signal.h>     // signal()

void handle_sigint(int signum) {
    printf("SIGINT received, but ignoring it in parent!\n");
}

int main(int argc, char* argv[]) {
    pid_t ret;

    // Set up signal handler to ignore SIGINT in parent
    signal(SIGINT, handle_sigint);

    printf("Forking sleeper...\n");

    ret = fork();
    if (ret == -1) {
        perror("Could not fork");
        exit(-1);
    }

    if (ret == 0) {  // Child process
        int exec_ret;
        char* cmd = "./sleep";
        char* myargv[] = {"sleep", NULL};
        exec_ret = execvp(cmd, myargv);
        if (exec_ret == -1) {
            perror("Error calling execvp");
            exit(-1);
        }
    }

    // Parent process
    printf("Waiting for sleeper %d...\n", ret);
    waitpid(ret, NULL, 0);

    printf("Parent finished waiting and returned successfully!\n");

    return 0;
}
