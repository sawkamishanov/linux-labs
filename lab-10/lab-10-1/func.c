#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define KEY 128
#define SIZE_BUF 24

typedef struct {
    char buf[SIZE_BUF];
    int front;
    int rear;
} buffer;

typedef struct {
    buffer buf;
    //
    //
} shared;

int id_shm = -1;

/* Запрос на разделяемый сегмент памяти */
int req_shm();
/* Включение разделяемой памяти в пространство процесса */
void* in_shm(int);
/* Полное уничтожение разделяемого сегмента */
int dest_shm(int);


int req_shm() {
    int id_shm;
    if ((id_shm = shmget((key_t)KEY, sizeof(shared), (0666 | IPC_CREAT))) == -1) {
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