#include "proc_scan.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int scan_proc(pid_list_t *out) {
  DIR *proc_dir;
  struct dirent *entry;
    
  if (pidlist_init(out, 65536) != 0) {
    fprintf(stderr, "[proc_scan] ERROR: out of memory\n");
    return -1;
  }

  proc_dir = opendir(PROC_PATH);
  if (proc_dir == NULL) {
    printf("[proc_scan] Error opening %s\n", PROC_PATH);
    return -1;
  }

  while((entry = readdir(proc_dir)) != NULL) {
    if (!is_numeric(entry->d_name)) continue;

    pid_t candidate = (pid_t)atoi(entry->d_name);
    pidlist_add(out,candidate);
  }

  closedir(proc_dir);
  fprintf(stdout, "[proc_scan] found %d PIDs in /proc\n", out->count);
  return 0;
}   


