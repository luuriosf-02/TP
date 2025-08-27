#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // para sleep

/* Allocate a temporary buffer. */
void* allocate_buffer(size_t size) {
    void* p = malloc(size);
    if (!p) {
        perror("malloc");
    } else {
        printf("[Hilo] Buffer de %zu bytes asignado en %p\n", size, p);
    }
    return p;
}

/* Deallocate a temporary buffer. */
void deallocate_buffer(void* buffer) {
    printf("[Hilo] Liberando buffer en %p\n", buffer);
    free(buffer);
}

/* Work function that uses a temporary buffer and a cleanup handler. */
void do_some_work(void) {
    /* Allocate a temporary buffer. */
    void* temp_buffer = allocate_buffer(1024);

    /* Register a cleanup handler for this buffer. */
    pthread_cleanup_push(deallocate_buffer, temp_buffer);

    /* Simulamos trabajo (y posible cancelación) */
    printf("[Hilo] Trabajando...\n");
    sleep(2);  // simula que hace algo

    /* Unregister the cleanup handler.
       1 => ejecuta la limpieza ahora mismo. */
    pthread_cleanup_pop(1);
}

/* Función que ejecuta el hilo */
void* thread_fn(void* arg) {
    (void)arg;
    do_some_work();
    printf("[Hilo] Finalizó normalmente.\n");
    return NULL;
}

int main(void) {
    pthread_t t;

    printf("[Main] Creando hilo...\n");
    if (pthread_create(&t, NULL, thread_fn, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    /* Si quisieras probar la cancelación, descomentá estas líneas:
    sleep(1);
    printf("[Main] Cancelando el hilo!\n");
    pthread_cancel(t);
    */

    pthread_join(t, NULL);
    printf("[Main] El hilo terminó.\n");

    return 0;
}

