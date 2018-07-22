#include "lab.h"
#include "shell.h"

job_list *jobs;

int main(int argc, const char **argv) {
    if (!(jobs = NewJobList())) {
        FStrErr("NewJobList error");
    }
    char command[1024];
    Signal(SIGINT, SigHandler);
    Signal(SIGCHLD, SigHandler);
    Signal(SIGTSTP, SigHandler);
    for (;;) {
        printf("> ");
        FGets(command, 1024, stdin);
        if (feof(stdin)) {
            exit(0);
        }
        if (strcmp(command, "\n") == 0) {
            continue;
        }
        Eval(command);
    }
    return 0;
}