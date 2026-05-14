#include "dirent_scan.h"

#include <stdio.h>          
#include <string.h>       
#include <stdlib.h>      
#include <fcntl.h>      
#include <unistd.h>    
#include <sys/syscall.h>
#include <sys/types.h> 
#include <stdint.h>   

struct linux_dirent64{
  uint64_t d_ino;
  int64_t d_off;
  uint16_t d_reclen;
  uint8_t d_type;
  char d_name[];
};

#define DENTS_BUF_SIZE 4096

int scan_dirent(pid_list_t *out) {
  int fd;
  char buf[DENTS_BUF_SIZE];
  int nread;

  fd = open(PROC_PATH, O_RDONLY | O_DIRECTORY);
  if (fd < 0 ) {
    fprintf(stderr, "[dirent_scan] ERROR: cannot open %s\n" , PROC_PATH);
    return -1;
  }

  if (pidlist_init(out, 65536) != 0) {
    fprintf(stderr, " [dirent_scan] ERROR: out of memory\n");
    close(fd);
    return -1;
  }

  while(1) {
    nread = syscall(SYS_getdents64, fd , buf, DENTS_BUF_SIZE);

    if (nread < 0) {
      fprintf(stderr, "[dirent_scan] ERROR: getdents64 failed\n");
      close(fd);
      return -1;

    }
     
    if (nread == 0 ) break;

    int pos = 0;
    while(pos < nread) { 
      struct linux_dirent64 *entry =  (struct linux_dirent64 *)(buf + pos);

      if (is_numeric(entry->d_name)) {
        pid_t candidate = (pid_t)atoi(entry->d_name);
        if (pidlist_add(out, candidate) != 0) {
          fprintf(stderr, " [dirent_scan] WARNING: list full\n");
          goto done;
        }
      }

      pos += entry->d_reclen;
    }
  }

done:
  close(fd);
  fprintf(stdout, "[dirent_scan] found %d PIDs in /proc\n", out->count);
  return 0;
}
