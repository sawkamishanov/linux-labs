#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "sync.c"
                    // KEYS 999 888 777 1021 2066 121 122 124 126 128
#define KEY 128
#define PATH "text.txt"
#define SIZE_BUF 1024

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


int open_file() {
    if ((file = fopen(PATH, "a")) == NULL) {
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

void print_pid(const char* path, FILE* file) {
    pid_t pid = getpid();
    fprintf(file,
    "%s:\n\
    PID (идентификатор процесса): %d\n parent PID (идентификатор предка): %d\n\
    SID (идентификатор сессии процеса): %d\n PGID (идентификатор группы процесса): %d\n\
    UID (реальный идентификатор пользователя): %d\n EUID (эффективный идентификатор пользователя): %d\n\
    GID (реальный групповой идентификатор): %d\n EGID (эффективный групповой идентификатор): %d\n\n",
    path, pid, getppid(), getsid(pid), getpgid(pid), getuid(), geteuid(), getgid(), getegid());
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