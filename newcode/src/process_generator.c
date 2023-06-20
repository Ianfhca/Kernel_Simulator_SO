#include "../defines/process_generator.h"

void generate_process(pcb_t *pcb, int num_process) {
    pcb->pid = num_process;
    pcb->state = 0;
    pcb->live_time = (rand() % 80) + 20;  // random value between 20 and 100
}

/**
 * This function counts clock pulses and generates a processe when pulses reach established frecuence.
 * Parameters:
 *  - *arg: arg_t type struct
 * Return:
 */
void *timer1(void *arguments) {
    pthread_mutex_lock(&mutex);
    args_t *args = arguments;
    pcb_t pcb;
    int pulses = 0;
    int num_process = 0;
    int freq_pgen = args->freq_pgen[0] + rand() % (args->freq_pgen[1] - args->freq_pgen[0] + 1);

    while (1) {
        args->done++;
        pulses++;
        if (pulses == freq_pgen) {
            pulses = 0;
            num_process++;
            generate_process(&pcb, num_process);
            freq_pgen = args->freq_pgen[0] + rand() % (args->freq_pgen[1] - args->freq_pgen[0] + 1);
            printf("Timer 1 has interrupt, pid: %d\n", pcb.pid);
            fflush(stdout);
        }
        pthread_cond_signal(&cond1);
        pthread_cond_wait(&cond2, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}