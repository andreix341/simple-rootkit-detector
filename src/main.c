#include <stdio.h>
#include <stdlib.h>
#include "../include/dirent_scan.h"
#include "../include/kill_scan.h"
#include "../include/proc_scan.h"
#include "../include/compare.h"

int main(void)
{
    pid_list_t proc_list;
    pid_list_t kill_list;
    pid_list_t dirent_list;

    scan_proc(&proc_list);
    scan_kill(&kill_list);
    scan_dirent(&dirent_list);
    
    sort_pid_list(&proc_list);
    sort_pid_list(&kill_list); 
    sort_pid_list(&dirent_list);
    int sus = compare_results(&kill_list, &proc_list, &dirent_list);

    pidlist_free(&proc_list);
    pidlist_free(&kill_list);
    pidlist_free(&dirent_list);
    printf("Total suspicious PIDs: %d \n\n", sus);
    return 0;
}
