#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/* semaphore POSIX */
#include <semaphore.h>

#define PATH "text.txt"
#define KEY 400
#define WRITE_COUNT 5

typedef struct {
    sem_t write;
    sem_t read;
    sem_t mutex_1;
    sem_t mutex_2;
    int write_count;
    int read_count;
    int init;
} shared;

FILE* file = NULL;
int id_shm = -1;

/* Работа с файлом */
int open_file();
int close_file();
/* Запрос на разделяемый сегмент памяти */
int req_shm();
/* Включение разделяемой памяти в пространство процесса */
void* in_shm(int);
/* Полное уничтожение разделяемого сегмента */
int dest_shm(int);

int open_file(char* mode) {
    if ((file = fopen(PATH, mode)) == NULL) {
        perror("open file");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int close_file() {
    if (fclose(file) == EOF) {
        perror("close file");
        return -1;
    }
    return 0;
}

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