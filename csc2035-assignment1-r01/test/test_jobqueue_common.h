/******** DO NOT EDIT THIS FILE ********/
/* 
 * test_jobqueue_common.h - structures and function declarations for
 * common unit tests of pri_jobqueue, ipc_jobqueue and sem_jobqueue functions.
 */  
#ifndef _TEST_JOBQUEUE_COMMON_H
#define _TEST_JOBQUEUE_COMMON_H
#define MUNIT_ENABLE_ASSERT_ALIASES
#include <stdbool.h>
#include "munit/munit.h"
#include "../sim_config.h"
#include "../job.h"
#include "../pri_jobqueue.h"

/* common test struct with jobqueue operations */
typedef struct test_jq {
    /* this queue to test */
    void* q;
    /* underlying queue implementation */
    pri_jobqueue_t* qimpl;
    /* pointers to queue functions to test */
    job_t* (*dequeue)(void*, job_t*);
    void (*enqueue)(void*, job_t*);
    bool (*is_empty)(void*);
    bool (*is_full)(void*);
    job_t* (*peek)(void*, job_t*);
    int (*size)(void*);
    int (*space)(void*);
} test_jq_t;

// utility functions
void init_job(job_t* job);
void set_job(job_t* job, pid_t pid, unsigned int id, unsigned int priority);
bool jobs_initialised(job_t* jobs, int buf_size, int empty_slots);
bool jobs_valid(job_t* jobs, int buf_size, int used_slots);
void assert_queue_initialised(pri_jobqueue_t* q);
bool equal_jobs(job_t* j1, job_t* j2);

// tests
MunitResult test_pjq_init(test_jq_t* test_jq);

MunitResult test_jq_ndequeue_randpri(test_jq_t* test_jq);
MunitResult test_jq_ndequeue_samepri(test_jq_t* test_jq);
MunitResult test_jq_ndequeue_decpri(test_jq_t* test_jq);
MunitResult test_jq_ndequeue_incpri(test_jq_t* test_jq);
MunitResult test_jq_ndequeue_inout(test_jq_t* test_jq);
MunitResult test_jq_ndequeue_randinout(test_jq_t* test_jq);
MunitResult test_jq_ndequeue_heap(test_jq_t* test_jq);

MunitResult test_jq_dequeue_empty(test_jq_t* test_jq);
MunitResult test_jq_dequeue_null(test_jq_t* test_jq);

MunitResult test_jq_enqueue_full(test_jq_t* test_jq);
MunitResult test_jq_enqueue_null(test_jq_t* test_jq);
MunitResult test_jq_enqueue_zeropri(test_jq_t* test_jq);

MunitResult test_jq_is_empty(test_jq_t* test_jq);
MunitResult test_jq_is_empty_notempty(test_jq_t* test_jq);
MunitResult test_jq_is_empty_nqnotempty(test_jq_t* test_jq);
MunitResult test_jq_is_empty_null(test_jq_t* test_jq);

MunitResult test_jq_is_full_empty(test_jq_t* test_jq);
MunitResult test_jq_is_full_filling(test_jq_t* test_jq);
MunitResult test_jq_is_full_nqfilling(test_jq_t* test_jq);
MunitResult test_jq_is_full_null(test_jq_t* test_jq);

MunitResult test_jq_peek_full(test_jq_t* test_jq);
MunitResult test_jq_peek_empty(test_jq_t* test_jq);
MunitResult test_jq_peek_heap(test_jq_t* test_jq);
MunitResult test_jq_peek_null(test_jq_t* test_jq);

MunitResult test_jq_size_empty(test_jq_t* test_jq);
MunitResult test_jq_size_filling(test_jq_t* test_jq);
MunitResult test_jq_size_nqfilling(test_jq_t* test_jq);
MunitResult test_jq_size_null(test_jq_t* test_jq);

MunitResult test_jq_space_empty(test_jq_t* test_jq);
MunitResult test_jq_space_filling(test_jq_t* test_jq);
MunitResult test_jq_space_nqfilling(test_jq_t* test_jq);
MunitResult test_jq_space_null(test_jq_t* test_jq);
#endif