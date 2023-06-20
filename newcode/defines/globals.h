#ifndef GLB
#define GLB

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "structs.h"

#define NUM_TIMERS 2
#define NUM_THREADS 3
#define MAX_PROCESS 5000

pid_t gettid(void);

extern pthread_mutex_t mutex;
extern pthread_cond_t cond1;
extern pthread_cond_t cond2;

extern int num_cpus, num_cores, num_threads;

extern int process_map[];

#endif