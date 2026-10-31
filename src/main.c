#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUMBER_OF_CARS 5

int main() {

    printf("=== PARKING MANAGEMENT SYSTEM ===\n");
    printf("Creating %d cars...\n\n", NUMBER_OF_CARS);

    for (int i = 1; i <= NUMBER_OF_CARS; i++) {

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {

            printf("Car %d: Process started! PID = %d\n",
                   i, getpid());

            sleep(2);

            printf("Car %d: Process finished!\n", i);

            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for all cars

    for (int i = 0; i < NUMBER_OF_CARS; i++) {
        wait(NULL);
    }

    printf("\nAll cars have finished.\n");
    printf("Parking simulation completed!\n");

    return 0;
}