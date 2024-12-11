/*
 * Replace the following string of 0s with your student number
 * 230057999
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "joblog.h"

/* 
 * DO NOT EDIT the new_log_name function. It is a private helper 
 * function provided for you to create a log name from a process 
 * descriptor for use when reading, writing and deleting a log file.
 * 
 * You must work out what the function does in order to use it properly
 * and to clean up after use.
 */
static char* new_log_name(proc_t* proc) {
    static char* joblog_name_fmt = "%s/%.31s%07d.txt";
                                // string format for the name of a log file
                                // declared static to have only one instance

    if (!proc)
        return NULL;

    char* log_name;
            
    asprintf(&log_name, joblog_name_fmt, JOBLOG_PATH, proc->type_label,
        proc->id);

    return log_name;
}

/* 
 * DO NOT EDIT the joblog_init function that sets up the log directory 
 * if it does not already exist.
 */
int joblog_init(proc_t* proc) {
    if (!proc) {
        errno = EINVAL;
        return -1;
    }
        
    int r = 0;
    if (proc->is_init) {
        struct stat sb;
    
        if (stat(JOBLOG_PATH, &sb) != 0) {
            errno = 0;
            r = mkdir(JOBLOG_PATH, 0777);
        }  else if (!S_ISDIR(sb.st_mode)) {
            unlink(JOBLOG_PATH);
            errno = 0;
            r = mkdir(JOBLOG_PATH, 0777);
        }
    }

    joblog_delete(proc);    // in case log exists for proc
    
    return r;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - you have to go to the beginning of the line represented
 *      by entry_num to read the required entry
 * - see job.h for a function to create a job from its string representation
 */
job_t* joblog_read(proc_t* proc, int entry_num, job_t* job) {

    // If no process
    if (!proc) {
        return NULL;
    }

    // If negative entry number
    if (entry_num<0) {
        return NULL;
    }

    // If no job dynamically allocate one
    if (!job) {
        job=(job_t*)malloc(sizeof(job_t));
    }

    // Generate log name
    char*log_name = new_log_name(proc);

    // If no file name generated
    if (!log_name) {
        return NULL;
    }

    // Attempt open file
    FILE* fp = fopen(log_name, "r");

    // If fail to open file
    if (!fp) {
        errno=0;
        free(log_name);
        return NULL;
    }

    // Init 
    char* entry = NULL;
    size_t len = 0;
    ssize_t read = 0;
    size_t count = 0;

    // Iterate through file / while not at end of file
    while ((read=getline(&entry, &len, fp)) != -1) {
        // If found
        if (count==entry_num) {
            sscanf(entry,JOB_STR_FMT,&job->pid,&job->id,&job->priority,job->label);
            return job;
            }
        // Increment count
        count ++;
    }

    fclose(fp);
    // returns null if entry_num > file size and subsequently nothing found
    return NULL;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - remember new entries are appended to a log file
 * - see the hint for joblog_read
 */
void joblog_write(proc_t* proc, job_t* job) {

    if (!proc || !job) {
        return;
    }

    // Pointer to log name
    char*log_name = new_log_name(proc);

    // If no file name generated
    if (!log_name) {
        return;
    }

    // Open files
    FILE* fp = fopen(log_name, "a");

    // If fail to open file
    if (!fp) {
        free(log_name);
        return;
    }

    // Storing string
    char log_line[JOB_STR_SIZE];

    // Format log line
    snprintf(log_line, JOB_STR_SIZE, JOB_STR_FMT, job->pid, job->id, job->priority, job->label);

    // Write in file
    fprintf(fp,"%s\n",log_line);

    // Close the file
    fclose(fp);

    // Free log
    free(log_name);

}

/* 
 * TODO: you must implement this function.
 */
void joblog_delete(proc_t* proc) {
    // Check proc if exists
    if (!proc) {
        return;
    }

    // Delete file
    unlink(new_log_name(proc));

}