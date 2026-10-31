# Parking Management System

## English

### 1. Project Description

The Parking Management System is a C program that simulates a parking lot with limited capacity using UNIX processes and POSIX semaphores.

The parking lot has 3 available spaces and 5 cars. Each car is represented by a separate process. If the parking lot is full, additional cars must wait until a space becomes available.

### 2. Technologies

- C programming language
- UNIX processes (`fork()`)
- Process synchronization
- POSIX named semaphores
- `waitpid()` for process management
- GCC compiler

### 3. Project Structure

```text
Parking-management-system/
│
├── src/
│   ├── main.c
│   ├── parking.c
│   └── parking.h
│
├── README.md
└── .gitignore
```

### 4. How It Works

1. The main process initializes the parking semaphore.
2. Five child processes are created using `fork()`.
3. Each car requests a parking space.
4. If a space is available, the car enters the parking lot.
5. If the parking lot is full, the car waits using `sem_wait()`.
6. When a car leaves, `sem_post()` releases a parking space.
7. A waiting car can then enter.
8. The parent process waits for all child processes using `waitpid()`.
9. Semaphore resources are closed and removed after execution.

### 5. Compilation and Execution

Compile the program:

```bash
gcc -Wall -Wextra -std=c11 src/main.c src/parking.c -o parking -pthread
```

Run the program:

```bash
./parking
```

### 6. Simulation Parameters

- Parking capacity: 3 spaces
- Total number of cars: 5
- Parking duration: 2 seconds per car
- Process creation: `fork()`
- Synchronization: POSIX named semaphore

### 7. Testing

The program was compiled and tested on macOS.

The test confirmed that:
- Five child processes are created.
- No more than three cars occupy the parking lot simultaneously.
- Additional cars wait when the parking lot is full.
- Waiting cars enter when spaces become available.
- All child processes terminate successfully.
- Semaphore resources are cleaned up.

---

## Italiano

### 1. Descrizione del progetto

Il Parking Management System è un programma scritto in C che simula un parcheggio con capacità limitata utilizzando i processi UNIX e i semafori POSIX.

Il parcheggio dispone di 3 posti e 5 automobili. Ogni automobile è rappresentata da un processo separato. Quando il parcheggio è pieno, le altre automobili devono attendere fino a quando non si libera un posto.

### 2. Tecnologie utilizzate

- Linguaggio di programmazione C
- Processi UNIX (`fork()`)
- Sincronizzazione dei processi
- Semafori POSIX con nome
- `waitpid()` per la gestione dei processi
- Compilatore GCC

### 3. Struttura del progetto

```text
Parking-management-system/
│
├── src/
│   ├── main.c
│   ├── parking.c
│   └── parking.h
│
├── README.md
└── .gitignore
```

### 4. Funzionamento del programma

1. Il processo principale inizializza il semaforo del parcheggio.
2. Vengono creati cinque processi figli utilizzando `fork()`.
3. Ogni automobile richiede un posto nel parcheggio.
4. Se è disponibile un posto, l'automobile può entrare.
5. Se il parcheggio è pieno, l'automobile attende utilizzando `sem_wait()`.
6. Quando un'automobile esce, `sem_post()` libera un posto.
7. Un'automobile in attesa può quindi entrare.
8. Il processo principale attende la terminazione di tutti i processi figli utilizzando `waitpid()`.
9. Le risorse del semaforo vengono chiuse e rimosse al termine dell'esecuzione.

### 5. Compilazione ed esecuzione

Compilare il programma:

```bash
gcc -Wall -Wextra -std=c11 src/main.c src/parking.c -o parking -pthread
```

Eseguire il programma:

```bash
./parking
```

### 6. Parametri della simulazione

- Capacità del parcheggio: 3 posti
- Numero totale di automobili: 5
- Durata della sosta: 2 secondi per automobile
- Creazione dei processi: `fork()`
- Sincronizzazione: semaforo POSIX con nome

### 7. Test

Il programma è stato compilato e testato su macOS.

Il test ha confermato che:
- Vengono creati cinque processi figli.
- Non più di tre automobili occupano contemporaneamente il parcheggio.
- Le automobili aggiuntive attendono quando il parcheggio è pieno.
- Le automobili in attesa entrano quando si liberano dei posti.
- Tutti i processi figli terminano correttamente.
- Le risorse del semaforo vengono rilasciate.
