#ifndef CSAPP_LAB
#define CSAPP_LAB

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#define MAX_LINE 1024

void Kill(pid_t pid, int sig);

char * FGets(char *, int, FILE *);

void FStrErr(char *);

void UnixErr(char *);

pid_t Fork(void);

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

__sighandler_t  Signal(int sigNum, __sighandler_t handler);
#endif