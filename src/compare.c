#include "compare.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int compare_results(const pid_list_t *kill_list, const pid_list_t *proc_list, const pid_list_t *dirent_list)
{
  int suspicious = 0;

  printf("\n ========== Rootkit detector report ========== \n");

  for ( int i = 0; i < kill_list->count; i++) {
    pid_t pid = kill_list->pids[i];

    int in_proc = pid_in_list(proc_list, pid);
    int in_dirent = dirent_list ? pid_in_list(dirent_list, pid) : -1;

    if (!in_proc && !in_dirent){
      printf("[CRITICAL] PID %d - hidden from both /proc and getdents64\n", pid);
      suspicious++;
    } else if (!in_proc){
      printf("[WARNING] PID %d - hidden from /proc\n", pid);
    }
  }


  for (int i = 0; i < proc_list->count; i++){
    pid_t pid = proc_list->pids[i];
    if(!pid_in_list(kill_list, pid)) printf("[INFO] PID %d - in /proc but not confirmed by kill \n", pid);
  }

  return suspicious;
}

int compare_pids(const void *a, const void *b)
{
  pid_t pid_a = *(pid_t *)a;
  pid_t pid_b = *(pid_t *)b;

  return pid_a - pid_b;
}

void sort_pid_list(pid_list_t *list)
{
  qsort(list->pids, list->count, sizeof(pid_t), compare_pids);
} 

int pid_in_list(const pid_list_t *list, pid_t pid)
{
  int low = 0;
  int high = list->count - 1;

  while(low <= high){
    int mid = (low + high) /2;
    
    if (list->pids[mid] == pid) return 1;
    else if ( list->pids[mid] < pid) low = mid + 1;
    else high = mid - 1;
  }
  return 0;
}
