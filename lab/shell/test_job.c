#include "lab.h"
#include "shell.h"


int main(int argc, char const *argv[]) {
    job_list *jobs = NewJobList();
    job_node *j1 = NewJob(1, 1, 1, "job1","job1");
    job_node *j2 = NewJob(2, 2, 2, "job2","job1");
    job_node *j3 = NewJob(3, 3, 3, "job3","job1");
    job_node *j4 = NewJob(4, 4, 4, "job4","job1");
    job_node *j5 = NewJob(5, 5, 5, "job5","job1");
    job_node *j6 = NewJob(6, 6, 6, "job6","job1");
    job_node *j7 = NewJob(7, 7, 7, "job7","job1");
    job_node *j8 = NewJob(8, 8, 8, "job8","job1");
    job_node *j9 = NewJob(9, 9, 9, "job9","job1");
    job_node *j10 = NewJob(10, 10, 10, "job10","job1");
    AppendJob(jobs, j1);
    AppendJob(jobs, j2);
    AppendJob(jobs, j3);
    AppendJob(jobs, j4);
    AppendJob(jobs, j5);
    AppendJob(jobs, j6);
    AppendJob(jobs, j7);
    AppendJob(jobs, j8);
    AppendJob(jobs, j9);
    AppendJob(jobs, j10);
    job_node *node;
    DelJob(jobs, 1);
    DelJob(jobs, 3);
    DelJob(jobs, 2);
    DelJob(jobs, 4);
    DelJob(jobs, 5);
    DelJob(jobs, 6);
    DelJob(jobs, 7);
    for (node = jobs->head; node != NULL; node = node->next) {
        printf("%s\n", node->name);
    }
    return 0;
}
