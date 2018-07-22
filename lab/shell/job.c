#include "lab.h"
#include "shell.h"

job_list *NewJobList() {
    job_list *jobs;
    if (!(jobs = (job_list *) malloc(sizeof(job_list))))
        return NULL;
    jobs->len = 0;
    jobs->head = jobs->tail = NULL;
    return jobs;
}

job_node *NewJob(uid_t uid, pid_t ppid, pid_t pid, char *name, char *cmd) {
    if (name == NULL || pid <= 0) return NULL;
    job_node *newNode = (job_node *) malloc(sizeof(job_node));
    newNode->uid = uid;
    newNode->pid = pid;
    newNode->ppid = ppid;
    newNode->name = name;
    newNode->cmd = cmd;
    newNode->next = NULL;
    return newNode;
}


void AppendJob(job_list *jobs, job_node *job) {
    if (jobs->head == NULL) {
        jobs->head = jobs->tail = job;
    } else {
        jobs->tail->next = job;
        jobs->tail = job;
    }
    ++jobs->len;
}

job_node *FindJob(job_list *jobs, pid_t pid) {
    job_node *node;
    for (node = jobs->head; node != NULL && node->pid != pid; node = node->next);
    return node;
}

job_node *DelJob(job_list *jobs, pid_t pid) {
    job_node *head, *freeNode;
    head = jobs->head;
    if (head == NULL) {
        return NULL;
    }
    if (head->pid == pid) {
        freeNode = head;
        jobs->head = head->next;
        --jobs->len;
        return freeNode;
    }
    for (; head != NULL; head = head->next) {
        if (head->next != NULL && head->next->pid == pid) {
            freeNode = head->next;
            head->next = freeNode->next;
            --jobs->len;
            return freeNode;
        }
    }
    return NULL;
}

void DestroyJobs(job_list *jobs) {
    job_node *freeNode;
    while (jobs->head != NULL) {
        freeNode = jobs->head;
        jobs->head = jobs->head->next;
        free(freeNode);
    }
    free(jobs);
}