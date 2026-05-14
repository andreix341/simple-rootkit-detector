#include "kill_scan.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

static int is_process(pid_t pid)
{
    char  path[64];
    char  line[256];
    FILE *f;
    int   tid = -1, tgid = -1;

    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    f = fopen(path, "r");

    if (!f) {
        return 0;
    }

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "Pid:%d",  &tid);
        sscanf(line, "Tgid:%d", &tgid);
        if (tid != -1 && tgid != -1) break;
    }
    fclose(f);

    if (tid == -1 || tgid == -1) return 0;
    return (tid == tgid);
}

int scan_kill(pid_list_t *out) {
  pid_t pid;
  pid_t pid_max = 32768;
  FILE *f;

  f = fopen(PID_MAX_PATH, "r");
  if (f) {
    fscanf(f, "%d", &pid_max);
    fclose(f);
  }
  // fprintf(stdout, "[kill_scan] pid_max = %d\n", pid_max);
  
  if (pidlist_init(out, pid_max) != 0) { 
    fprintf(stderr, "[kill_scan] ERROR: out of memory\n");
    return -1;
  }

  for (pid = 1; pid <= pid_max; pid++) {
    if ( kill(pid, 0) == 0) goto record;
    if (errno == EPERM) goto record;
    continue;

  record:
    if (!is_process(pid)) continue;
    if (pidlist_add(out, pid) != 0) {
      fprintf(stderr, "[kill_scan] WARNING: list full\n");
      break;
    }

  }
  fprintf(stdout, "[kill_scan] Found %d PIDs via kill\n", out->count);
  return 0;
}
