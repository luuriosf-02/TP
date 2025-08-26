#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

struct job {
    struct job* next;
    // Otros campos que describen el trabajo
};

/* Lista de trabajos pendientes */
struct job* job_queue = NULL;

/* Mutex que protege job_queue */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Semáforo que cuenta la cantidad de trabajos */
sem_t job_queue_count;

/* Prototipo de la función que procesa un trabajo */
void process_job(struct job* j);

/* Inicialización de la cola */
void initialize_job_queue()
{
    job_queue = NULL;
    sem_init(&job_queue_count, 0, 0);
}

/* Función del thread que procesa trabajos */
void* thread_function(void* arg)
{
    while (1) {
        struct job* next_job;

        /* Esperar hasta que haya un trabajo disponible */
        sem_wait(&job_queue_count);

        /* Bloquear la cola */
        pthread_mutex_lock(&job_queue_mutex);

        /* Tomar el siguiente trabajo (el semáforo asegura que hay al menos uno) */
        next_job = job_queue;
        job_queue = job_queue->next;

        /* Desbloquear la cola */
        pthread_mutex_unlock(&job_queue_mutex);

        /* Procesar y liberar el trabajo */
        process_job(next_job);
        free(next_job);
    }
    return NULL;
}

/* Agregar un nuevo trabajo a la cola */
void enqueue_job(/* datos del trabajo */)
{
    struct job* new_job = (struct job*)malloc(sizeof(struct job));
    // Inicializar campos de new_job según los datos del trabajo

    pthread_mutex_lock(&job_queue_mutex);
    new_job->next = job_queue;
    job_queue = new_job;
    sem_post(&job_queue_count); // Indicar que hay un trabajo disponible
    pthread_mutex_unlock(&job_queue_mutex);
}

/* Ejemplo simple de process_job */
void process_job(struct job* j) {
    printf("Procesando un trabajo...\n");
}

/* Ejemplo de main */
int main()
{
    initialize_job_queue();

    /* Crear un thread consumidor */
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);

    /* Encolar algunos trabajos */
    for (int i = 0; i < 5; ++i) {
        enqueue_job();
    }

    /* Esperar al thread */
    pthread_join(thread, NULL);

    /* Destruir semáforo al final */
    sem_destroy(&job_queue_count);

    printf("Todos los trabajos procesados.\n");
    return 0;
}

