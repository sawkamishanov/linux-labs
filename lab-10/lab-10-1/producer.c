#include "func.c"


int main(int argc, char* argv[]) {
    int id_shm = req_shm();
    void* shmaddr = in_shm(id_shm);
    shared* shvar = (shared*)shmaddr;

    /*  Инициализация буфера */
    shvar->buf.front = 0;
    shvar->buf.rear = 0;

    /* Инициализация семафоров */
    sem_init(&(shvar->empty), 1, SIZE_BUF);
    sem_init(&(shvar->full), 1, 0);

    /* Открытие файла */
    open_file();

    char ch;
    while (1) {
        sem_wait(&(shvar->empty));
        if ((ch = fgetc(file)) != EOF) {
            shvar->buf.buf[shvar->buf.front] = ch;
            shvar->buf.front = (shvar->buf.front + 1) % SIZE_BUF;
        } else {
            break;
        }
        sem_post(&(shvar->full));
        sleep(2);
    }

    dest_shm(id_shm);
    close_file();

    return EXIT_SUCCESS;
}