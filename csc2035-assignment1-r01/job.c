/*
 * Replace the following string of 0s with your student number
 * 230057999
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "job.h"

/* 
 * DO NOT EDIT the job_new function.
 */
job_t* job_new(pid_t pid, unsigned int id, unsigned int priority, 
    const char* label) {
    return job_set((job_t*) malloc(sizeof(job_t)), pid, id, priority, label);
}

/* 
 * TODO: you must implement this function
 */
job_t* job_copy(job_t* src, job_t* dst) {
    
    // If src null, else if src=dst, else if 
    if (!src) {
        return NULL;
    } 
    
    if (strlen(src->label)!=MAX_NAME_SIZE-1) {
        return NULL;
    } 
    
    if (!dst) {
        // Allocate new job
        job_t* job = (job_t* )malloc(sizeof(job_t));
        *job = *src;
        return job;
    } 
    
    if (src==dst) {
        // Return either pointer
        return src;
    }
    *dst=*src;
    return dst;
}

/* 
 * TODO: you must implement this function
 */
void job_init(job_t* job) {
    // If job is null
    if (!job) {
        return;
    }
    // Set fields
    job->pid =0;
    job->id=0;
    job->priority=0;
    strncpy(job->label,PAD_STRING,MAX_NAME_SIZE);

}

/* 
 * TODO: you must implement this function
 */
bool job_is_equal(job_t* j1, job_t* j2) {

    // Check if both NULL
    if (!j1 & !j2) {
        return true;
    }

    // Check if either are NULL
    if (!j1 | !j2) {
        return false;
    }

    // Compare j1 and j2
    return ((j1->pid == j2->pid) & (j1->id==j2->id) & (j1->priority==j2->priority) & (strncmp(j1->label, j2->label, MAX_NAME_SIZE) ==0));
}

/*
 * TODO: you must implement this function.
 * Hint:
 * - read the information in job.h about padding and truncation of job
 *      labels
 */
job_t* job_set(job_t* job, pid_t pid, unsigned int id, unsigned int priority,
    const char* label) {
    
    // If job is null return null pointer
    if (!job) {
        return NULL;
    }
    // Set job attributes
    job->id = id;
    job->pid = pid;
    job->priority=priority;

    // If label null
    if (!label) {
        strncpy(job->label,PAD_STRING,MAX_NAME_SIZE);
    } else {
        // Copy label
        strncpy(job->label,label,MAX_NAME_SIZE-1);
        int padding_size = 0;
        // Calculate padding size
        if (MAX_NAME_SIZE>strlen(label)+1) {
            // Set padding size
            padding_size = MAX_NAME_SIZE - (strlen(label)+1);
        }
        
        // Concatinate pad string
        strncat(job->label,PAD_STRING,padding_size);
    }

    return job;
}

/*
 * TODO: you must implement this function.
 * Hint:
 * - see malloc and calloc system library functions for dynamic allocation, 
 *      and the documentation in job.h for when to do dynamic allocation
 */
char* job_to_str(job_t* job, char* str) {

    // If str size is NULL
    if (!job) {
        return NULL;
    }

    // If str size is NULL
    if (!str) {
        str = (char*)malloc(JOB_STR_SIZE*sizeof(char));
        // If dynamic allocation fails
        if (!str) {
            return NULL;
        }
    }

    // check if label length doesn't equal max name size
    if (strlen(job->label)!=MAX_NAME_SIZE-1) {
        return NULL;
    }

    // Formatting
    sprintf(str,JOB_STR_FMT,job->pid,job->id,job->priority,job->label);

    return str;

}

/*
 * TODO: you must implement this function.
 * Hint:
 * - see the hint for job_to_str
 */
job_t* str_to_job(char* str, job_t* job) {

    // If str size is NULL
    if (!str) {
        return NULL;
    }

    // If job size is NULL
    if (!job) {
        job = (job_t*)malloc(sizeof(job_t));
        // If dynamic allocation fails
        if (!job) {
            return NULL;
        }
    }

    // check if label length doesn't equal max name size
    if (strlen(str)!=JOB_STR_SIZE-1) {
        return NULL;
    }

    // Formatting
    int n = sscanf(str,JOB_STR_FMT,&job->pid,&job->id,&job->priority,job->label);

    if (n!=4) {
        // Not all passed
        return NULL;
    }

    return job;

}

/* 
 * TODO: you must implement this function
 * Hint:
 * - look at the allocation of a job in job_new
 */
void job_delete(job_t* job) {
    // if job is null
    if (!job) {
        return;
    }
    free(job);

}