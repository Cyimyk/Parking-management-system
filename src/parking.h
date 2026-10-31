// dichiarazioni delle funzioni del parcheggio, usate da main.c

#ifndef PARKING_H
#define PARKING_H

// crea il semaforo con "capacity" posti, si chiama una volta sola prima dei fork
void parking_init(int capacity);

// l'auto prova a entrare: se non c'è posto aspetta
void parking_enter(int car_id);

// l'auto esce e libera un posto
void parking_leave(int car_id);

// chiude e rimuove il semaforo, la chiama il padre alla fine
void parking_destroy(void);

#endif
