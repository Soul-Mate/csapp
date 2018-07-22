#include "lab.h"
#include "shell.h"

extern char **environ;

extern job_list *jobs; // from main job_list;

static char *Shell_Command[] = {
        "quit", "ls", "pwd", "cd", "clear", "sleep"
};

static int Shell_Command_Len = 4;

static int parseCommand(char *buf, size_t commandLen, char **argv, int *argc);

static void movetoSpace(char *buf);

static int isBuiltInCommand(char **);

static void runBuiltInCommand(char **argv);

static int isShellCommand(const char *);

static void runCommand(int bg, int argc, char **argv);

void Eval(char *command) {
    int bg;
    int argc;
    char *buf;
    char *argv[128];
    size_t commandLen = strlen(command);

    if (!(buf = malloc(commandLen))) {
        FStrErr("Eval error");
    }

    strcpy(buf, command);
    bg = parseCommand(buf, commandLen, argv, &argc);
    if (argv[0] == NULL) return;
    if (isBuiltInCommand(argv)) {
        runBuiltInCommand(argv);
    } else {
        runCommand(bg, argc, argv);
    }
}

static int isBuiltInCommand(char **argv) {
    if (strcmp(argv[0], "quit") == 0) return 1;
    if (strcmp(argv[0], "jobs") == 0) return 1;
    if (strcmp(argv[0], "bg") == 0) return 1;
    if (strcmp(argv[0], "fg") == 0) return 1;
    return 0;
}

static void runBuiltInCommand(char **argv) {
    if (strcmp(argv[0], "quit") == 0) {
        QuitCommand();
        return;
    }
    if (strcmp(argv[0], "jobs") == 0) {
        JobsCommand();
        return;
    }

    if (strcmp(argv[0], "fg") == 0) {
        fgCommand(argv);
        return;
    }
}

static int isShellCommand(const char *command) {
    int i;
    for (i = 0; i < Shell_Command_Len; i++) {
        if (strcmp(Shell_Command[i], command) == 0) return 1;
    }
    return 0;
}

static int parseCommand(char *buf, size_t commandLen, char **argv, int *argc) {
    char *delimit;
    int bg = 0;
    *argc = 0;
    buf[commandLen - 1] = ' ';
    movetoSpace(buf);
    while ((delimit = strchr(buf, ' '))) {
        argv[(*argc)++] = buf;
        *delimit = '\0';
        buf = delimit + 1;
        movetoSpace(buf);
    }
    argv[*argc] = NULL;
    if (strcmp(argv[*argc - 1], "&") == 0) {
        bg = 1;
        argv[--*argc] = NULL;
    }
    return bg;
}

static void movetoSpace(char *buf) {
    while (*buf && (*buf == CHAR_SPACE)) buf++;
}

static char *cmdCat(int argc, char **argv) {
    int i;
    int argvLen = 0;
    for (i = 0; i < argc; i++) {
        argvLen += strlen((argv)[i]) + 2;
    }
    char *cmd = (char *) malloc(sizeof(char) * argvLen);
    for (i = 0; i < argc; i++) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }
    return cmd;
}

static void runCommand(int bg, int argc, char **argv) {
    pid_t pid;
    sigset_t maskOne, prevOne;
    sigemptyset(&maskOne);
    // 阻塞SIGCHLD信号
    sigaddset(&maskOne, SIGCHLD);
    Sigprocmask(SIG_BLOCK, &maskOne, &prevOne);
    if ((pid = Fork()) == 0) {
        sleep(20);
        // 子进程会继承父进程信号集合
        // 因此在调用execvp之前解除阻塞
        Sigprocmask(SIG_SETMASK, &prevOne, NULL);
        if (execvp(argv[0], argv) == -1) {
            UnixErr("exec error");
        }
    }
    // 在添加进程到链表前阻塞所有到达的信号
    // 添加完成后解除,当子进程退出前
    // 继续阻塞后续到达的SIGCHLD信号
    sigset_t maskAll, prevAll;
    sigfillset(&maskAll);
    Sigprocmask(SIG_BLOCK, &maskAll, &prevAll);
    job_node *newJob = NewJob(getuid(), getppid(), pid, argv[0], cmdCat(argc, argv));
    if (newJob == NULL) {
        Sigprocmask(SIG_SETMASK, &prevAll, NULL);
        FStrErr("newJob error");
    }
    AppendJob(jobs, newJob);
    Sigprocmask(SIG_SETMASK, &prevAll, NULL);
    Sigprocmask(SIG_SETMASK, &prevOne, NULL);
    // 后台任务
    if (bg) {
        printf("(%s: %d) runing\n", argv[0], pid);
    } else {
        SetFgPid(pid);
        while (GetFgPid()) {
            sigsuspend(&prevOne);
        }
    }

}