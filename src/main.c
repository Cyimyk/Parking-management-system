// simulazione di un parcheggio con 3 posti e 5 auto
// ogni auto è un processo figlio creato con fork()
// il padre crea i figli, li aspetta e pulisce il semaforo

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

    // PID dei figli, serve al padre per fare waitpid
    pid_t car_processes[NUMBER_OF_CARS];

    printf("=== PARKING MANAGEMENT SYSTEM ===\n");
    printf("Parking capacity: %d spaces\n", PARKING_CAPACITY);
    printf("Total number of cars: %d\n\n", NUMBER_OF_CARS);

    // il semaforo si crea prima dei fork così i figli lo ereditano
    parking_init(PARKING_CAPACITY);

    // crea un processo per ogni auto
    for (int i = 0; i < NUMBER_OF_CARS; i++) {

        // fork: nel figlio ritorna 0, nel padre il PID del figlio, -1 se fallisce
        pid_t pid = fork();

        // fork fallita: aspetta i figli già creati, pulisce ed esce
        if (pid < 0) {
            perror("Fork failed");

            for (int j = 0; j < i; j++) {
                waitpid(car_processes[j], NULL, 0);
            }

            parking_destroy();
            exit(EXIT_FAILURE);
        }

        // codice del figlio (l'auto)
        if (pid == 0) {

            int car_id = i + 1;

            // stato: auto partita
            printf("Car %d: Process started! PID = %d\n",
                   car_id, getpid());
            fflush(stdout);

            // stato: chiede un posto, entra o aspetta
            parking_enter(car_id);

            // stato: parcheggiata, il posto resta occupato 2 secondi
            sleep(2);

            // stato: esce e libera il posto
            parking_leave(car_id);

            printf("Car %d: Process finished!\n", car_id);
            fflush(stdout);

            // il figlio finisce qui, senza exit rientrerebbe nel for
            exit(EXIT_SUCCESS);
        }

        // codice del padre: salva il PID del figlio
        car_processes[i] = pid;
    }

    // il padre aspetta che tutti i figli terminino
    for (int i = 0; i < NUMBER_OF_CARS; i++) {

        // se waitpid viene interrotta da un segnale (EINTR) riprova
        while (waitpid(car_processes[i], NULL, 0) == -1) {

            if (errno == EINTR) {
                continue;
            }

            perror("waitpid failed");
            parking_destroy();
            exit(EXIT_FAILURE);
        }
    }

    // tutte le auto hanno finito, rimuove il semaforo
    parking_destroy();

    printf("\nAll cars have finished.\n");
    printf("Parking simulation completed!\n");

    return 0;
}
