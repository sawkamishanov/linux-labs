#include "func.c"


int main(int argc, char* argv[]) {
    int id_shm = req_shm();
    void* shmaddr = in_shm(id_shm);
    shared* shvar = (shared*)shmaddr;

    /*  Инициализация буфера */
    shvar->buf.front = 0;
    shvar->buf.rear = 0;
    memset(shvar->buf.buf, 0, SIZE_BUF);

    /* Инициализация семафоров */
    sem_init(&(shvar->empty), 1, SIZE_BUF);
    sem_init(&(shvar->full), 1, 0);
    sem_init(&(shvar->end), 1, 0);

    /* Открытие файла */
    open_file();

    char ch;
    do {
        ch = fgetc(file);
        sem_wait(&(shvar->empty));
        printf("front: %d\n", shvar->buf.front);
        printf("rear: %d\n", shvar->buf.rear);
        shvar->buf.buf[shvar->buf.front] = ch;
        shvar->buf.front = (shvar->buf.front + 1) % SIZE_BUF;
        sem_post(&(shvar->full));
        //sleep(1);
    } while(ch != EOF);

    printf("Ожидание...\n");
    sem_wait(&(shvar->end));

    dest_shm(id_shm);
    close_file();

    return EXIT_SUCCESS;
}