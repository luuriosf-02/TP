#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Calcula números primos sucesivos (ineficientemente). 
   Retorna el N-ésimo número primo, donde N es el valor en *arg. */
void* compute_prime(void* arg) {
    int candidate = 2;
    int n = *((int*) arg);

    while (1) {
        int factor;
        int is_prime = 1;

        /* Verifica primalidad por división sucesiva */
        for (factor = 2; factor < candidate; ++factor) {
            if (candidate % factor == 0) {
                is_prime = 0;
                break;
            }
        }

        /* ¿Es el primo que buscamos? */
        if (is_prime) {
            if (--n == 0) {
                int* result = malloc(sizeof(int)); // reservar memoria
                *result = candidate;
                return result; // devolvemos un puntero a int
            }
        }
        ++candidate;
    }
    return NULL;
}

int main() {
    pthread_t thread;
    int which_prime = 5000;
    int* prime;

    /* Crear el hilo para calcular el 5000° número primo */
    pthread_create(&thread, NULL, compute_prime, &which_prime);

    /* Esperar el resultado del hilo */
    pthread_join(thread, (void**) &prime);

    /* Imprimir el resultado */
    printf("El %d° número primo es %d.\n", which_prime, *prime);

    /* Liberar memoria reservada en el hilo */
    free(prime);

    return 0;
}

