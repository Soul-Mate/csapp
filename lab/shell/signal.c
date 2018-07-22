#include "lab.h"
#include "shell.h"

extern job_list *jobs; // from main job_list;

static volatile sig_atomic_t fg_pid;

static void sigChildHandler();

static void sigTSTPHandler();

static void sigINTHandler();

void SigHandler(int sigNum) {
    switch (sigNum) {
        case SIGCHLD:
            sigChildHandler();
            break;
        case SIGINT:
            sigINTHandler();
            break;
        case SIGTSTP:
            sigTSTPHandler();
    }
}

static void sigChildHandler() {
    pid_t pid;
    int status;
    int oldErrno = errno;
    sigset_t mask, omask;
    sigfillset(&mask);
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        if (WIFEXITED(status)) {
            SetFgPid(0);
            Sigprocmask(SIG_BLOCK, &mask, &omask);
            job_node *node = DelJob(jobs, pid);
            free(node);
            Sigprocmask(SIG_SETMASK, &omask, NULL);
        }
        if (WIFSTOPPED(status)) {
            SetFgPid(0);
            Sigprocmask(SIG_BLOCK, &mask, &omask);
            DelJob(jobs, pid);
            Sigprocmask(SIG_SETMASK, &omask, NULL);
        }
        if (WIFSIGNALED(status)) {
            SetFgPid(0);
            Sigprocmask(SIG_BLOCK, &mask, &omask);
            DelJob(jobs, pid);
        }
        if (WIFCONTINUED(status)) {
            SetFgPid(pid);
            Sigprocmask(SIG_BLOCK, &mask, &omask);
            Sigprocmask(SIG_SETMASK, &omask, NULL);
        }
    }
    errno = oldErrno;
}

static void sigINTHandler() {
    if (fg_pid == 0) {
        Signal(SIGINT, SIG_DFL);
        kill(getpid(), SIGINT);
    } else {
        kill(fg_pid, SIGINT);
    }
}

static void sigTSTPHandler() {
    if (fg_pid == 0) {
        Signal(SIGTSTP, SIG_DFL);
        kill(getpid(), SIGTSTP);
    } else {
        kill(GetFgPid(), SIGTSTP);
    }
}


void SetFgPid(sig_atomic_t pid) {
    fg_pid = pid;
}

sig_atomic_t GetFgPid() {
    return fg_pid;
}