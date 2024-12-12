/*
 * Replace the following string of 0s with your student number
 * 230057999
 */
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "pri_jobqueue.h"

/* 
 * TODO: you must implement this function that allocates a job queue and 
 * initialise it.
 * Hint:
 * - see job_new in job.c
 */
pri_jobqueue_t* pri_jobqueue_new() {

    // Dynamically allocate memory
    pri_jobqueue_t* pjq = (pri_jobqueue_t*)malloc(sizeof(pri_jobqueue_t));
    
    // If call fails because pjq is null
    if (!pjq) {
        // On failure return NULL
        return NULL;
    }

    // Init pjq
    pri_jobqueue_init(pjq);

    return pjq;
}

/* 
 * TODO: you must implement this function.
 */
void pri_jobqueue_init(pri_jobqueue_t* pjq) {

    // Set fields
    pjq->buf_size=JOB_BUFFER_SIZE;
    pjq->size=0;
    // Iterating through jobs
    for (int i = 0; i < pjq->buf_size; i++) {
        // Init job
        job_init(&pjq->jobs[i]);
    }   

}

/* 
 * TODO: you must implement this function.
 * Hint:
 *      - if a queue is not empty, and the highest priority job is not in the 
 *      last used slot on the queue, dequeueing a job will result in the 
 *      jobs on the queue having to be re-arranged
 *      - remember that the job returned by this function is a copy of the job
 *      that was on the queue
 */
job_t* pri_jobqueue_dequeue(pri_jobqueue_t* pjq, job_t* dst) {

    // If pjq is null or empty
    if (!pjq) {
        return NULL;
    }

    if(pri_jobqueue_is_empty(pjq)){
        return NULL;
    }

    // If dst is null
    if (!dst) {
        // Dynamically allocate memory
        dst = (job_t*)malloc(sizeof(job_t));
        // If call fails
        if (!dst) {
            return NULL;
        }
    }

    // Create index and job_lp var
    int index = -1;
    int job_lp = -1;

    // Iterate through to find highest priority job
    for (int i = 0; i < pjq->buf_size; i++) {
        job_t *job=&pjq->jobs[i];
        // If job_lp not set
        if (job_lp==-1) {
            job_lp=job->priority;
            index=i;
        }
        // If slot empty
        if (job->priority>0 && job->priority < job_lp) {
            // Set priority
            job_lp=job->priority;
            // Set index
            index=i;
        }
    }

    // Set dst
    job_copy(&pjq->jobs[index],dst);
    // Init high priority job
    job_init(&pjq->jobs[index]);
    // Decrement size
    pjq->size--;
    // Ensure queue in right order
    // Iterate through to find highest priority job
    for (int i = index; i < pjq->buf_size-1; i++) {
        // Set index
        pjq->jobs[i] = pjq->jobs[i+1];
    }

    // Init last item
    job_init(&pjq->jobs[pjq->buf_size-1]);

    // Return dst pointer
    return dst;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - if a queue is not full, and if you decide to store the jobs in 
 *      priority order on the queue, enqueuing a job will result in the jobs 
 *      on the queue having to be re-arranged. However, it is not essential to
 *      store jobs in priority order (it simplifies implementation of dequeue
 *      at the expense of extra work in enqueue). It is your choice how 
 *      you implement dequeue (and enqueue) to ensure that jobs are dequeued
 *      by highest priority job first (see pri_jobqueue.h)
 * - remember that the job passed to this function is copied to the 
 *      queue
 */
void pri_jobqueue_enqueue(pri_jobqueue_t* pjq, job_t* job) {

    // If pjq null or job pointer null or if invalid job priority (not int or lower/equal to 0)
    if (!pjq || !job) {
        return;
    }

    // If pjq full
    if (pri_jobqueue_is_full(pjq)) {
        return;
    }

    if (job->priority <= 0) {
        return;
    }

    // Iterate through
    for (int i = 0; i < pjq->buf_size; i++) {
        // If slot empty
        if (pjq->jobs[i].priority==0) {
            // Add job to queue
            job_copy(job,&pjq->jobs[i]);
            // Increment number of jobs in q
            pjq->size++;
            // Return
            return;
        }
    }  

}
   
/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_empty(pri_jobqueue_t* pjq) {
    // If queue null or if no jobs in the queue
    if (!pjq) {
        return true;
    }

    if (pjq->size==0){
        return true;
    }

    // Else
    return false;
}

/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_full(pri_jobqueue_t* pjq) {

    // If queue null or queue size equals buffer size
    if (!pjq) {
        return true;
    }
    
    // If size = buf size
    if (pjq->size==pjq->buf_size) {
        return true;
    }

    // Else
    return false;
    
    }

/* 
 * TODO: you must implement this function.
 * Hints:
 *      - remember that the job returned by this function is a copy of the 
 *      highest priority job on the queue.
 *      - both pri_jobqueue_peek and pri_jobqueue_dequeue require copying of 
 *      the highest priority job on the queue
 */
job_t* pri_jobqueue_peek(pri_jobqueue_t* pjq, job_t* dst) {

    // If pjq null or queue is empty
    if (!pjq || pri_jobqueue_is_empty(pjq)) {
        return NULL;
    }

    // If dst is null
    if (!dst) {
        // Dynamically allocate memory
        dst = (job_t*)malloc(sizeof(job_t));
    }

    job_t job_lp;

    // Iterate through
    for (int i = 0; i < pjq->buf_size; i++) {
        job_t job=pjq->jobs[i];
        // Find highest priority job
        if (job.priority>0 && job.priority < job_lp.priority) {
            job_lp = job;
        }
    }

    // Copy to and return dst
    job_copy(&job_lp,dst);
    return dst;

}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_size(pri_jobqueue_t* pjq) {

    // If pjq null
    if (!pjq) {
        // No space
        return 0;
    }

    // Create counter var
    int count = 0;

    // Iterate through
    for (int i = 0; i < pjq->buf_size; i++) {
        // If slot empty
        if (pjq->jobs[i].priority>0) {
            // Increment count for job
            count++;
        }
    }  

    return count;

}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_space(pri_jobqueue_t* pjq) {

    // If pjq null
    if (!pjq) {
        // No space
        return 0;
    }

    // Return buf size - size
    return pjq->buf_size-pri_jobqueue_size(pjq);

}

/* 
 * TODO: you must implement this function.
 *  Hint:
 *      - see pri_jobqeue_new
 */
void pri_jobqueue_delete(pri_jobqueue_t* pjq) {

    // If pjq is null
    if (!pjq) {
        // No effect
        return;
    }

    // Free memory allocated to queue
    free(pjq);

}
