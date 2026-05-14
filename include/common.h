#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define PROC_PATH "/proc"
#define PID_MAX_PATH "/proc/sys/kernel/pid_max"

typedef struct {
    pid_t *pids;
    int count;
    int capacity;
} pid_list_t;


static inline int pidlist_init(pid_list_t *list, int capacity)
{
  list->pids = malloc(sizeof(pid_t) * capacity);
  if (!list->pids) return -1;
  list->count = 0;
  list->capacity = capacity;
  return 0;
}

static inline int pidlist_add(pid_list_t *list, pid_t pid)
{
  if (list->count >= list->capacity) return -1;
  list->pids[list->count++] = pid;
  return 0;
}

static inline void pidlist_free(pid_list_t *list)
{
  free(list->pids);
  list->pids = NULL;
  list->count = 0;
  list->capacity = 0;
}

static inline int is_numeric(const char *str) {
    if (!str || *str == '\0') return 0;
    for(; *str; str++) 
        if (!isdigit((unsigned char) *str)) return 0;
    return 1;
}

#endif
