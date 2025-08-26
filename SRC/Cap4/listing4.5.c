#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Función que ejecuta el hilo */
void* thread_function(void* thread_arg) {
    printf("Hilo ejecutándose...\n");
    /* Aquí iría el trabajo del hilo */
    return NULL; // necesario para cumplir el contrato de pthread
}

int main() {
    pthread_attr_t attr;
    pthread_t thread;

    /* Inicializar atributos */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    /* Crear hilo detached */
    if (pthread_create(&thread, &attr, thread_function, NULL) != 0) {
        perror("Error al crear el hilo");
        exit(1);
    }

    /* Destruir atributos */
    pthread_attr_destroy(&attr);

    /* El hilo principal sigue trabajando */
    printf("Hilo principal ejecutándose...\n");

    /* Dormimos un poco para que el hilo secundario alcance a ejecutar 
       (ya que es detached, no lo podemos join) */
    pthread_exit(NULL); 

    return 0; // nunca se alcanza si usamos pthread_exit
}

