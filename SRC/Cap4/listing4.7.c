#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Clave usada para asociar un log a cada hilo */
static pthread_key_t thread_log_key;

/* Escribe MESSAGE en el log del hilo actual */
void write_to_thread_log(const char* message) {
    FILE* thread_log = (FILE*) pthread_getspecific(thread_log_key);
    if (thread_log) {
        fprintf(thread_log, "%s\n", message);
        fflush(thread_log); // asegurar que se escriba
    }
}

/* Cierra el archivo de log asociado al hilo */
void close_thread_log(void* thread_log) {
    if (thread_log) {
        fclose((FILE*) thread_log);
    }
}

/* Función que ejecutan los hilos */
void* thread_function(void* args) {
    char thread_log_filename[128];
    FILE* thread_log;

    /* Nombre del archivo en /tmp con el ID del hilo */
    snprintf(thread_log_filename, sizeof(thread_log_filename),
             "/tmp/thread_%lu.log", (unsigned long) pthread_self());

    /* Abrir el archivo */
    thread_log = fopen(thread_log_filename, "w");
    if (!thread_log) {
        perror("Error al abrir log del hilo");
        return NULL;
    }

    /* Asociar el archivo al hilo */
    pthread_setspecific(thread_log_key, thread_log);

    /* Escribir al log */
    write_to_thread_log("Thread starting...");

    /* Aquí iría el trabajo real del hilo */
    write_to_thread_log("Thread finished.");

    return NULL;
}

int main() {
    int i;
    pthread_t threads[5];

    /* Crear la clave TLS (thread-local storage) */
    if (pthread_key_create(&thread_log_key, close_thread_log) != 0) {
        perror("Error creando clave pthread");
        return 1;
    }

    /* Crear hilos */
    for (i = 0; i < 5; ++i) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("Error creando hilo");
            return 1;
        }
    }

    /* Esperar a que terminen */
    for (i = 0; i < 5; ++i) {
        pthread_join(threads[i], NULL);
    }

    /* Liberar clave TLS */
    pthread_key_delete(thread_log_key);

    return 0;
}
