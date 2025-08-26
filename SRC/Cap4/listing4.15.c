#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Función del hilo */
void* thread_function(void* arg) {
    fprintf(stderr, "child thread pthread id is %lu\n", pthread_self());
    /* Bloquea el hilo sin consumir CPU */
    pause(); 
    return NULL;
}

int main() {
    pthread_t thread;
    int ret;

    fprintf(stderr, "main thread pthread id is %lu\n", pthread_self());

    ret = pthread_create(&thread, NULL, thread_function, NULL);
    if (ret != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    /* Bloquea el hilo principal sin consumir CPU */
    pause();  

    return 0;
}

