#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Flag global y sincronización */
int thread_flag;
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;

/* Función de ejemplo para simular trabajo */
void do_work() {
    printf("Doing work...\n");
}

/* Inicializa mutex, variable de condición y flag */
void initialize_flag() {
    pthread_mutex_init(&thread_flag_mutex, NULL);
    pthread_cond_init(&thread_flag_cv, NULL);
    thread_flag = 0;
}

/* Hilo que espera el flag y realiza trabajo */
void* thread_function(void* thread_arg) {
    while (1) {
        pthread_mutex_lock(&thread_flag_mutex);
        while (!thread_flag) {
            /* Espera bloqueado hasta que el flag se ponga a 1 */
            pthread_cond_wait(&thread_flag_cv, &thread_flag_mutex);
        }
        pthread_mutex_unlock(&thread_flag_mutex);

        /* Realiza el trabajo fuera del mutex */
        do_work();
    }
    return NULL;
}

/* Cambia el valor del flag y despierta hilos esperando */
void set_thread_flag(int flag_value) {
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_cond_signal(&thread_flag_cv); // o pthread_cond_broadcast si hay varios hilos
    pthread_mutex_unlock(&thread_flag_mutex);
}

/* Limpieza de recursos */
void cleanup_flag() {
    pthread_mutex_destroy(&thread_flag_mutex);
    pthread_cond_destroy(&thread_flag_cv);
}

/* Ejemplo de uso */
int main() {
    pthread_t thread;
    initialize_flag();
    pthread_create(&thread, NULL, thread_function, NULL);

    /* Simula activar el hilo después de 2 segundos */
    sleep(2);
    set_thread_flag(1);

    /* Espera para ver salida */
    sleep(2);

    cleanup_flag();
    return 0;
}

