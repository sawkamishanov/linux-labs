#define NUM_PROCESS 3
#define FIRST 0
#define SECOND 1
#define THIRD 2
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct shared_data {
    int choosing[NUM_PROCESS];
    int number[NUM_PROCESS];
    int stop;
} shared;

void lock(shared*, int);
void unlock(shared*, int);

/* Алгоритм пекарни для синхронизации трех процессов */
void lock(shared* shared_var, int process) {
    shared_var->choosing[process] = 1;
    shared_var->number[process] = 1 + MAX(shared_var->number[FIRST], 
                                        MAX(shared_var->number[SECOND], shared_var->number[THIRD]));
    shared_var->choosing[process] = 0;
    for (int i = 0; i < NUM_PROCESS; ++i) {
        if (i != process) {
            while (shared_var->choosing[i]);
            while (shared_var->number[i] != 0 && 
                (shared_var->number[process] > shared_var->number[i] ||
                    (shared_var->number[process] == shared_var->number[i] && process > i))); // + || (number[process] == number[i] && process > i))
        }
    }
    /* Критическая секция */
}

void unlock(shared* shared_var, int process) {
    shared_var->number[process] = 0;
}