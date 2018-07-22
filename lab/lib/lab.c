#include "lab.h"

char * FGets(char *str, int n, FILE *stream) {
    char *ret;
    if (str == NULL) return NULL;
    if ((ret = fgets(str, n, stream)) && ferror(stream)) FStrErr("FGets error");
    return ret;
}

void UnixErr(char *msg) {
    fprintf(stderr,"%s: %s\n", msg, strerror(errno));
    exit(0);
}

void FStrErr(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
    if (sigprocmask(how, set, oldset) == -1) {
        UnixErr("sigprocmask error");
    }
    return;
}

void Kill(pid_t pid, int sig) {
    if (kill(pid, sig) == -1) UnixErr("kill error");
}

pid_t Fork(void) {
    pid_t pid;
    if  ((pid = fork()) < 0)  
        UnixErr("fork error");
    return pid;
}

__sighandler_t Signal(int sigNum, __sighandler_t handler) {
    sigset_t set;
    sigemptyset(&set);
    struct sigaction act, oldAct;
    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;
    act.sa_mask = set;
    sigaction(sigNum, &act, &oldAct);
    return handler;
}

