#include "func.c"


int main(int argc, char* argv[]) {
    int id_shm = req_shm();
    void* shmaddr = in_shm(id_shm);
    shared* shvar = (shared*)shmaddr;

    open_file("r");
    char buf[512];
    while (1) {
        sleep(1);
        sem_wait(&(shvar->read));
        sem_wait(&(shvar->mutex_1));
        ++(shvar->read_count);
        if (shvar->read_count == 1) {
            sem_wait(&(shvar->write));
        }
        sem_post(&(shvar->mutex_1));
        sem_post(&(shvar->read));

        /* Чтение */
        if (fgets(buf, sizeof(buf), file) == NULL) {
            break;
        }
        printf("[Читатель PID:%d прочитал строку]: %s", getpid(), buf);
        sem_wait(&(shvar->mutex_1));
        --(shvar->read_count);
        if (shvar->read_count == 0) {
            sem_post(&(shvar->write));
        }
        sem_post(&(shvar->mutex_1));
    }

    dest_shm(id_shm);
    close_file();

    return EXIT_SUCCESS;
}