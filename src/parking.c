// gestione del parcheggio con un semaforo POSIX con nome
// il valore del semaforo = posti liberi (0 = parcheggio pieno)

#include "parking.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#define SEMAPHORE_NAME "/parking_management_system_sem"

// puntatore al semaforo, NULL finché non viene inizializzato
static sem_t *parking_semaphore = NULL;

// funzione che crea il semaforo con tanti posti quanti ne indica capacity
void parking_init(int capacity) {

    // capacity deve essere positiva
    if (capacity <= 0) {
        fprintf(stderr, "Invalid parking capacity.\n");
        exit(EXIT_FAILURE);
    }

    // cancella un eventuale semaforo rimasto da un'esecuzione precedente
    sem_unlink(SEMAPHORE_NAME);

    // crea il semaforo con valore iniziale = capacity
    parking_semaphore = sem_open(
        SEMAPHORE_NAME,
        O_CREAT | O_EXCL,
        0644,
        capacity
    );

    // se la creazione fallisce si esce
    if (parking_semaphore == SEM_FAILED) {
        perror("Semaphore initialization failed");
        parking_semaphore = NULL;
        exit(EXIT_FAILURE);
    }

    printf("Parking initialized. Capacity: %d spaces.\n\n",
           capacity);
    fflush(stdout);
}

// funzione con cui un'auto entra nel parcheggio
// se c'è posto entra subito, se è pieno aspetta
void parking_enter(int car_id) {

    // il semaforo deve essere già stato creato
    if (parking_semaphore == NULL) {
        fprintf(stderr, "Parking semaphore is not initialized.\n");
        exit(EXIT_FAILURE);
    }

    // prova a prendere un posto senza bloccarsi
    // se riesce il semaforo scende di 1 e l'auto è dentro
    if (sem_trywait(parking_semaphore) == 0) {

        printf("Car %d: Entered the parking lot.\n", car_id);
        fflush(stdout);
        return;
    }

    // EAGAIN vuol dire solo "semaforo a 0, parcheggio pieno"
    // qualsiasi altro errore è un problema vero
    if (errno != EAGAIN) {
        perror("sem_trywait failed");
        exit(EXIT_FAILURE);
    }

    // parcheggio pieno, l'auto si mette in attesa
    printf("Car %d: Parking is full. Waiting for a space...\n",
           car_id);
    fflush(stdout);

    // sem_wait blocca il processo finché un'altra auto non esce (sem_post)
    // se viene interrotta da un segnale (EINTR) riprova
    while (sem_wait(parking_semaphore) == -1) {

        if (errno == EINTR) {
            continue;
        }

        perror("sem_wait failed");
        exit(EXIT_FAILURE);
    }

    // si è liberato un posto e l'auto è entrata
    printf("Car %d: Entered the parking lot after waiting.\n",
           car_id);
    fflush(stdout);
}

// funzione con cui un'auto esce e libera il posto
void parking_leave(int car_id) {

    // il semaforo deve essere già stato creato
    if (parking_semaphore == NULL) {
        fprintf(stderr, "Parking semaphore is not initialized.\n");
        exit(EXIT_FAILURE);
    }

    printf("Car %d: Leaving the parking lot.\n", car_id);
    fflush(stdout);

    // sem_post fa salire il semaforo di 1 (posto libero)
    // se un'auto sta aspettando, si sveglia ed entra
    if (sem_post(parking_semaphore) == -1) {
        perror("sem_post failed");
        exit(EXIT_FAILURE);
    }

    printf("Car %d: Parking space released.\n", car_id);
    fflush(stdout);
}

// funzione che chiude e rimuove il semaforo a fine programma
void parking_destroy(void) {

    if (parking_semaphore != NULL) {

        // chiude il semaforo in questo processo
        if (sem_close(parking_semaphore) == -1) {
            perror("sem_close failed");
        }

        parking_semaphore = NULL;
    }

    // toglie il semaforo dal sistema, altrimenti resta anche dopo la fine
    if (sem_unlink(SEMAPHORE_NAME) == -1) {
        perror("sem_unlink failed");
    }

    printf("\nParking semaphore resources cleaned up.\n");
}
