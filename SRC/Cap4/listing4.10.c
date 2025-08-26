#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct job {
    struct job* next;
    // Otros campos de trabajo
};

// Lista de trabajos pendientes
struct job* job_queue = NULL;

// Mutex para proteger la cola
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

// Prototipo de función que procesa el trabajo
void process_job(struct job* j);

void* thread_function(void* arg)
{
    while (1) {
        pthread_mutex_lock(&queue_mutex);
        if (job_queue == NULL) {
            pthread_mutex_unlock(&queue_mutex);
            break; // No hay más trabajos
        }

        // Tomar el siguiente trabajo
        struct job* next_job = job_queue;
        job_queue = job_queue->next;
        pthread_mutex_unlock(&queue_mutex);

        // Procesar el trabajo
        process_job(next_job);

        // Liberar memoria
        free(next_job);
    }
    return NULL;
}

// Ejemplo simple de process_job
void process_job(struct job* j) {
    printf("Procesando trabajo...\n");
}

int main()
{
    // Crear algunos trabajos de ejemplo
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

