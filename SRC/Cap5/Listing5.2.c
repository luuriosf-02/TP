#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

int binary_semaphore_allocation(key_t key, int sem_flags) {
    return semget(key, 1, sem_flags);
}

int binary_semaphore_deallocate(int semid) {
    union semun ignored_argument;
    return semctl(semid, 0, IPC_RMID, ignored_argument);
}

int main() {
    key_t clave = 1234;
    int semid;

    printf("Creando un semaforo binario, espere...\n");

    semid = binary_semaphore_allocation(clave, IPC_CREAT | 0600);

    if (semid == -1) {
        perror("No se pudo crear el semaforo");
        return 1;
    }

    printf("Semaforo creado correctamente con ID = %d\n", semid);

    if (binary_semaphore_deallocate(semid) == -1) {
        perror("Error al eliminar el semáforo");
        return 1;
    }

    printf("Semaforo eliminado correctamente.\n");

    return 0;
}

