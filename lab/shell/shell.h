#ifndef CSAPP_SHELL
#define CSAPP_SHELL

#define CHAR_SPACE ' '

// job节点
typedef struct _job_node {
    uid_t uid;          // 用户id
    pid_t pid;          // 进程id
    pid_t ppid;         // 父进程id
    char *name;         // 任务名称
    char *cmd;          // 完整的任务名称
    u_int8_t status;    // 任务状态
    struct _job_node *next;
} job_node;

// job列表
typedef struct _job_list {
    int len;
    job_node *head;
    job_node *tail;
} job_list;


// 退出shell
void QuitCommand();

// 显示任务列表
void JobsCommand();

// fg %pid/pid
void fgCommand(char **argv);

// bg %pid/pid
void bgCommand(pid_t pid, char **argv);


// eval command
void Eval(char *Command);

// signal handler
void SigHandler(int sigNum);

sig_atomic_t GetFgPid();

void SetFgPid(sig_atomic_t pid);

// new job
job_node *NewJob(uid_t uid, pid_t ppid, pid_t pid, char *name, char *cmd);

job_list *NewJobList();

// add job
void AppendJob(job_list *jobs, job_node *job);

// del job by pid
job_node *DelJob(job_list *jobs, pid_t pid);

// find job by pid
job_node *FindJob(job_list *jobs, pid_t pid);

void DestroyJobs(job_list *jobs);

#endif