#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

int binary_semaphore_initialize(int semid) {
    union semun argument;
    unsigned short values[1];
    values[0] = 1;              
    argument.array = values;
    return semctl(semid, 0, SETALL, argument);
}

int main() {
    key_t clave = 1234; 
    int semid;

    printf("Creando semáforo binario...\n");

    semid = semget(clave, 1, IPC_CREAT | 0600);
    if (semid == -1) {
        perror("Error al crear semáforo");
        return 1;
    }
    printf("Semáforo creado con ID = %d\n", semid);

    if (binary_semaphore_initialize(semid) == -1) {
        perror("Error al inicializar semáforo");
        return 1;
    }
    printf("Semáforo inicializado con valor 1.\n");

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("Error al eliminar semáforo");
        return 1;
    }
    printf("Semáforo eliminado correctamente.\n");

    return 0;
}

