#include "init.h"
#include "process_queue.h"

void init_processmap() {
    int i;

    for (i = 0; i < MAX_PROCESS; i++) process_map[i] = 0; /*Init process_map*/
}

pcb_t init_pcb() {
    pcb_t null_process;
    null_process.pid = -1;
    null_process.status = NULL_S;
    null_process.load_quantum = -1;
    null_process.quantum = null_process.load_quantum;
    null_process.live_time = -1;
    null_process.priority = -1;
    return null_process;
}

void init_machine(args_t* args) {
    int cpu_id, core_id, thread_id;

    args->machine.num_cpus = num_cpus;
    args->machine.cpus = (cpu_t*)malloc(args->machine.num_cpus * sizeof(cpu_t)); /*CPUs*/
    printf("\n");
    for (cpu_id = 0; cpu_id < args->machine.num_cpus; cpu_id++) {
        cpu_t* cpu = &(args->machine.cpus[cpu_id]);
        cpu->id = cpu_id;
        cpu->num_cores = num_cores;
        printf("CPU %d:\n", cpu->id);

        cpu->cores = (core_t*)malloc(cpu->num_cores * sizeof(core_t)); /*Cores per CPU*/

        for (core_id = 0; core_id < cpu->num_cores; core_id++) {
            core_t* core = &(cpu->cores[core_id]);
            core->id = core_id;
            core->num_threads = num_threads;
            inic_queue(&core->queue);
            core->num_proc_queue = 0;
            printf(" -C%d -> ", core->id);

            core->threads = (thread_t*)malloc(core->num_threads * sizeof(thread_t)); /*Threads per core*/

            for (thread_id = 0; thread_id < core->num_threads; thread_id++) {
                thread_t* thread = &(core->threads[thread_id]);
                thread->cpu_id = cpu_id;
                thread->core_id = core_id;
                thread->id = thread_id;
                thread->pcb = init_pcb();
                printf("T%d ", thread->id);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int init_config(args_t* args) {
    char input = '\0';
    num_cpus = DEF_CPUS;
    num_cores = DEF_CORES;
    num_threads = DEF_THREADS;

    printf("+-----------{\033[1;37m KERNEL SIMULATOR\033[0m }-----------+\n");
    printf("| \033[1;37m  >                                 <   \033[0m |\n");
    printf("| \033[1;37m    * C (Configure simulator)           \033[0m |\n");
    printf("| \033[1;37m    * D (Launch default simulation)     \033[0m |\n");
    printf("| \033[1;37m    * R (Run simulation)                \033[0m |\n");
    printf("| \033[1;37m    * S (Stop simulation)               \033[0m |\n");
    printf("| \033[1;37m    * E (Exit)                          \033[0m |\n");
    printf("| \033[1;37m  >                                 <   \033[0m |\n");
    printf("+------------------->  <-------------------+\n");

    while (input != 'r' && input != 'R') {
        scanf(" %c", &input);

        switch (input) {
            case 'c':
            case 'C':
                printf("Set the number of CPUs: ");
                scanf("%d", &num_cpus);
                printf("Set the number of cores per CPU: ");
                scanf("%d", &num_cores);
                printf("Set the number of threads per core: ");
                scanf("%d", &num_threads);
                printf("Set the first frequence range of process generator: ");
                scanf("%d", &args->freq_pgen[0]);
                printf("Set the second frequence range of process generator: ");
                scanf("%d", &args->freq_pgen[1]);
                printf("Set the frequence of scheduler: ");
                scanf("%d", &args->freq_schl);
                printf("Machine has been configured successfully\n");
                break;
            case 'd':
            case 'D':
                num_cpus = DEF_CPUS;
                num_cores = DEF_CORES;
                num_threads = DEF_THREADS;
                args->freq_pgen[0] = 999;
                args->freq_pgen[1] = 3999;
                args->freq_schl = 5000;
                printf("\033[1;37m");
                printf("Default values has been set:\n");
                printf("CPUs: %d | Cores: %d | Threads: %d\n", num_cpus, num_cores, num_threads);
                printf("Process generator frequence: MIN %d / MAX %d \nScheduler/Dispatcher frequence %d\n", args->freq_pgen[0], args->freq_pgen[1], args->freq_schl);
                printf("\033[0m");
                input = 'r';
                break;
            case 'r':
            case 'R':
                printf("Running kernel simulator\n");
                break;
            case 's':
            case 'S':
                printf("First try to run the simulator\n");
                break;
            case 'e':
            case 'E':
                printf("Exit program\n");
                return (1);
                break;
            default:
                printf("(%c) is not a valid code. Please try again.\n", input);
                break;
        }
    }
    init_machine(args);
    init_processmap();
    return (0);
}