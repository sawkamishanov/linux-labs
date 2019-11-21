#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH "second"

typedef struct fork_delays {
    int parent_delay;
    int fork_delay;
    int vfork_delay;
} delay;

void fprint_pid(const char*, FILE*);
delay input_delays(void);

int main(int argc, char* argv[]) {
    FILE* file = NULL;
    delay dl;
    const int SIZE = 256;
    char path[SIZE];

    dl = input_delays();
    printf("Введите путь к файлу: ");
    scanf("%s", path);
    if ((file = fopen(path, "w")) == NULL) {
        perror("open file");
    }

    fprintf(file, 
            "Задержки\n Предок: %d sec, Потомок 1 (fork): %d sec, Потомок 2 (vfork): %d sec\n\n", 
            dl.parent_delay, dl.fork_delay, dl.vfork_delay);
    fclose(file);
    if (!fork()) {
        sleep(dl.fork_delay);
        file = fopen(path, "a");
        fprint_pid("Потомок 1 (fork)", file);
        fclose(file);
        exit(EXIT_SUCCESS);
    } else {
        if (!vfork()) {
            sleep(dl.vfork_delay);
            file = fopen(path, "a");
            fprint_pid("Потомок 2 (vfork)", file);
            fclose(file);
            execl(PATH, PATH, NULL);
            exit(EXIT_SUCCESS);
        }
    }
    sleep(dl.parent_delay);
    file = fopen(path, "a");
    fprint_pid("Предок", file);
    fclose(file);
    return 0;
}

void fprint_pid(const char* path, FILE* file) {
    pid_t pid = getpid();
    fprintf(file,
    "%s:\n\
    PID (идентификатор процесса): %d\n parent PID (идентификатор предка): %d\n\
    SID (идентификатор сессии процеса): %d\n PGID (идентификатор группы процесса): %d\n\
    UID (реальный идентификатор пользователя): %d\n EUID (эффективный идентификатор пользователя): %d\n\
    GID (реальный групповой идентификатор): %d\n EGID (эффективный групповой идентификатор): %d\n\n",
    path, pid, getppid(), getsid(pid), getpgid(pid), getuid(), geteuid(), getgid(), getegid());
}

delay input_delays() {
    delay dl;
    printf("Введите задержки\n");
    printf("Предок: ");
    scanf("%d", &dl.parent_delay);
    printf("Потомок (fork): ");
    scanf("%d", &dl.fork_delay);
    printf("Потомок (vfork): ");
    scanf("%d", &dl.vfork_delay);
    return dl;
}