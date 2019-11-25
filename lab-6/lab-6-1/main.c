#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PATH "cron_out.txt"

int main(int argc, char* argv[]) {
    FILE* file = NULL;
    time_t current_time = time(NULL);
    if ((file = fopen(PATH, "a")) == NULL) {
        perror("open file");
    }
    fprintf(file, "Время запуска программы: %s", ctime(&current_time));
    fclose(file);
}
