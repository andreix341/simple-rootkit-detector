#ifndef COMPARE_H
#define COMPARE_H

#include "common.h"

int compare_results(const pid_list_t *kill_list,
     const pid_list_t *proc_list,
     const pid_list_t *dirent_list);

int compare_pids(const void *a, const void *b);
void sort_pid_list(pid_list_t *list);
int pid_in_list(const pid_list_t *list, pid_t pid);

#endif
