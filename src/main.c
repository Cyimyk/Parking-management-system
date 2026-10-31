
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "parking.h"

#define NUMBER_OF_CARS 5
#define PARKING_CAPACITY 3

int main(void) {

    pid_t car_processes[NUMBER_OF_CARS];

    printf("=== PARKING MANAGEMENT SYSTEM ===\n");
    printf("Parking capacity: %d spaces\n", PARKING_CAPACITY);
    printf("Total number of cars: %d\n\n", NUMBER_OF_CARS);

    parking_init(PARKING_CAPACITY);

    for (int i = 0; i < NUMBER_OF_CARS; i++) {

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");

            for (int j = 0; j < i; j++) {
                waitpid(car_processes[j], NULL, 0);
            }

            parking_destroy();
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {

            int car_id = i + 1;

            printf("Car %d: Process started! PID = %d\n",
                   car_id, getpid());
            fflush(stdout);

            parking_enter(car_id);

            sleep(2);

            parking_leave(car_id);

            printf("Car %d: Process finished!\n", car_id);
            fflush(stdout);

            exit(EXIT_SUCCESS);
        }

        car_processes[i] = pid;
    }

    for (int i = 0; i < NUMBER_OF_CARS; i++) {

        while (waitpid(car_processes[i], NULL, 0) == -1) {

            if (errno == EINTR) {
                continue;
            }

            perror("waitpid failed");
            parking_destroy();
            exit(EXIT_FAILURE);
        }
    }

    parking_destroy();

    printf("\nAll cars have finished.\n");
    printf("Parking simulation completed!\n");

    return 0;
}
