
#include "lab.h"
#include "shell.h"

extern job_list *jobs;

static int parsePid(char *arg);

static int isNumber(char *arg);

void QuitCommand() {
    kill(getpgrp(), SIGINT);
}

void JobsCommand() {
    job_node *node;
    struct passwd *pwd;
    printf("USER\tUID\tPID\tPPID\tCMD\n");
    for (node = jobs->head; node != NULL; node = node->next) {
        if ((pwd = getpwuid(node->uid)) != NULL) {
            printf("%s\t%d\t%d\t%d\t%s\n",
                   pwd->pw_name,
                   pwd->pw_uid,
                   node->pid,
                   node->ppid,
                   node->cmd);
        }
    }
}

void fgCommand(char **argv) {
    int id;
    if ((id = parsePid(argv[1])) != -1 && argv[2] == NULL) {
        sigset_t mask, oldMask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGCHLD);
        Sigprocmask(SIG_BLOCK, &mask, &oldMask);
        if (argv[1][0] == '%') {
            job_node *node = FindJob(jobs, id);
            if (node == NULL) {
                printf("no found %d in job list\n", id);
                return;
            }
        }
        // 发送SIGCONT信号
        Kill(id, SIGCONT);
        SetFgPid(id);
        while (GetFgPid())
            sigsuspend(&oldMask);
        Sigprocmask(SIG_SETMASK, &oldMask, NULL);
    } else
        printf("format error, e.g. bg %%12  or  bg 1498\n");

}

void bgCommand(pid_t pid, char **argv) {
    int id;
    if ((id = parsePid(argv[1])) != -1 && argv[2] == NULL) {
        if (argv[1][0] == '%') {
            job_node *node = FindJob(jobs, id);
            if (node == NULL) {
                printf("no found %d in job list\n", id);
                return;
            }
        }
        // 发送SIGCONT信号
        kill(id, SIGCONT);
        printf("(%d) background runing\n", id);
    } else
        printf("format error, e.g. bg %%12  or  bg 1498\n");
}


static int isNumber(char *arg) {
    int i;
    size_t n;
    for (i = 0, n = strlen(arg); i < n; i++)
        if (!isdigit(arg[i])) return -1;
    return 1;
}

static int parsePid(char *arg) {
    int id;
    if (!isNumber(arg)) return -1;
    // 从运行中的任务列表中查找
    if (arg[0] == '%')
        return atoi(arg + 1);
    else
        return atoi(arg);
}

