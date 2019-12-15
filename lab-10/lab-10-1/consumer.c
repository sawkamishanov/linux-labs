#include "func.c"


int main(int argc, char* argv[]) {
    int id_shm = req_shm();
    void* shmaddr = in_shm(id_shm);
    shared* shvar = (shared*)shmaddr;

    printf("[Сообщение от поставщика]:\n");
    while (1) {
        sem_wait(&(shvar->full));
        if (shvar->buf.buf[shvar->buf.rear] == EOF) {
            break;
        }
        fflush(stdout);
        printf("%c", shvar->buf.buf[shvar->buf.rear]);
        shvar->buf.rear = (shvar->buf.rear + 1) % SIZE_BUF;
        sem_post(&(shvar->empty));
        sleep(1);
    }

    sem_post(&(shvar->end));

    sem_destroy(&(shvar->empty));
    sem_destroy(&(shvar->full));
    dest_shm(id_shm);

    return EXIT_SUCCESS;
}