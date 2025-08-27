#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct job {
    struct job* next;
    // Otros campos que describen el trabajo
};

/* Lista de trabajos pendientes */
struct job* job_queue = NULL;

/* Mutex que protege la cola */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Prototipo de la función que procesa un trabajo */
void process_job(struct job* j);

/* Función del thread para procesar la cola */
void* thread_function(void* arg)
{
    while (1) {
        struct job* next_job;

        /* Bloquear la cola para acceder de forma segura */
        pthread_mutex_lock(&job_queue_mutex);

        if (job_queue == NULL) {
            next_job = NULL;
        } else {
            next_job = job_queue;
            job_queue = job_queue->next;
        }

        pthread_mutex_unlock(&job_queue_mutex);

        /* Si la cola estaba vacía, terminamos el thread */
        if (next_job == NULL)
            break;

        /* Procesar el trabajo */
        process_job(next_job);

        /* Liberar memoria del trabajo */
        free(next_job);
    }
    return NULL;
}

/* Ejemplo de process_job */
void process_job(struct job* j) {
    printf("Procesando un trabajo...\n");
}

/* Ejemplo de main */
int main()
{
    /* Crear algunos trabajos de ejemplo */
    for (int i = 0; i < 5; ++i) {
        struct job* j = (struct job*)malloc(sizeof(struct job));
        j->next = job_queue;
        job_queue = j;
    }

    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, NULL);

    printf("Todos los trabajos procesados.\n");
    return 0;
}

