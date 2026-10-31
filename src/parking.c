
#include "parking.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#define SEMAPHORE_NAME "/parking_management_system_sem"

static sem_t *parking_semaphore = NULL;

void parking_init(int capacity) {

    if (capacity <= 0) {
        fprintf(stderr, "Invalid parking capacity.\n");
        exit(EXIT_FAILURE);
    }

    sem_unlink(SEMAPHORE_NAME);

    parking_semaphore = sem_open(
        SEMAPHORE_NAME,
        O_CREAT | O_EXCL,
        0644,
        capacity
    );

    if (parking_semaphore == SEM_FAILED) {
        perror("Semaphore initialization failed");
        parking_semaphore = NULL;
        exit(EXIT_FAILURE);
    }

    printf("Parking initialized. Capacity: %d spaces.\n\n",
           capacity);
    fflush(stdout);
}

void parking_enter(int car_id) {

    if (parking_semaphore == NULL) {
        fprintf(stderr, "Parking semaphore is not initialized.\n");
        exit(EXIT_FAILURE);
    }

    if (sem_trywait(parking_semaphore) == 0) {

        printf("Car %d: Entered the parking lot.\n", car_id);
        fflush(stdout);
        return;
    }

    if (errno != EAGAIN) {
        perror("sem_trywait failed");
        exit(EXIT_FAILURE);
    }

    printf("Car %d: Parking is full. Waiting for a space...\n",
           car_id);
    fflush(stdout);

    while (sem_wait(parking_semaphore) == -1) {

        if (errno == EINTR) {
            continue;
        }

        perror("sem_wait failed");
        exit(EXIT_FAILURE);
    }

    printf("Car %d: Entered the parking lot after waiting.\n",
           car_id);
    fflush(stdout);
}

void parking_leave(int car_id) {

    if (parking_semaphore == NULL) {
        fprintf(stderr, "Parking semaphore is not initialized.\n");
        exit(EXIT_FAILURE);
    }

    printf("Car %d: Leaving the parking lot.\n", car_id);
    fflush(stdout);

    if (sem_post(parking_semaphore) == -1) {
        perror("sem_post failed");
        exit(EXIT_FAILURE);
    }

    printf("Car %d: Parking space released.\n", car_id);
    fflush(stdout);
}

void parking_destroy(void) {

    if (parking_semaphore != NULL) {

        if (sem_close(parking_semaphore) == -1) {
            perror("sem_close failed");
        }

        parking_semaphore = NULL;
    }

    if (sem_unlink(SEMAPHORE_NAME) == -1) {
        perror("sem_unlink failed");
    }

    printf("\nParking semaphore resources cleaned up.\n");
}
