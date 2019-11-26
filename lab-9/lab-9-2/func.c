#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define KEY_1 142
#define KEY_2 144
#define SIZE_BUF 16
#define RUN_COUNT 5

typedef struct {
    int number[SIZE_BUF];
    int lock;
} shared;

int id_shm_1 = -1;
int id_shm_2 = -1;

/* Запрос на разделяемый сегмент памяти */
int req_shm(int);
/* Включение разделяемой памяти в пространство процесса */
void* in_shm(int);
/* Полное уничтожение разделяемого сегмента */
int dest_shm(int);


int req_shm(int key) {
    int id_shm;
    if ((id_shm = shmget((key_t)key, sizeof(shared), (0666 | IPC_CREAT))) == -1) {
        perror("req_shm -> shmget");
        return -1;
    }

    return id_shm;
}

void* in_shm(int id_shm) {
    void* shmaddr = NULL;
    if (*(int*)(shmaddr = shmat(id_shm, NULL, 0)) == -1) {
        perror("shmat");
        return (void*)-1;
    }

    return shmaddr;
}

int und_shm(void* shmaddr) {
    if (shmdt(shmaddr) == -1) {
        perror("und_shm -> shmdt");
        return -1;
    }
    return 0;
}

int dest_shm(int id_shm) {
    if (shmctl(id_shm, IPC_RMID, NULL) == -1) {
        perror("dest_shm -> shmctl+IPC_RMID");
        return -1;
    }

    return 0;
}