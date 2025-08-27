#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* Definición de union semun necesaria */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Esperar (P / wait) */
int binary_semaphore_wait(int semid) {
    struct sembuf operations[1];
    operations[0].sem_num = 0;  
    operations[0].sem_op = -1;  
    operations[0].sem_flg = SEM_UNDO;  
    return semop(semid, operations, 1);  
}

/* Liberar (V / post) */
int binary_semaphore_post(int semid) {
    struct sembuf operations[1];
    operations[0].sem_num = 0;  
    operations[0].sem_op = 1;  
    operations[0].sem_flg = SEM_UNDO;  
    return semop(semid, operations, 1);  
}

/* Inicializar semáforo en 1 */
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
    printf("Semáforo inicializado en 1\n");

    printf("Ejecutando WAIT (tomando el semáforo)...\n");
    if (binary_semaphore_wait(semid) == -1) {
        perror("Error en wait");
        return 1;
    }
    printf("Semáforo tomado, valor debería ser 0 ahora.\n");

    printf("Ejecutando POST (liberando el semáforo)...\n");
    if (binary_semaphore_post(semid) == -1) {
        perror("Error en post");
        return 1;
    }
    printf("Semáforo liberado, valor debería ser 1 otra vez.\n");

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("Error al eliminar semáforo");
        return 1;
    }
    printf("Semáforo eliminado correctamente.\n");

    return 0;
}

