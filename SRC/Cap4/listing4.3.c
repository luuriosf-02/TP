#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Estructura que contiene los parámetros para la función del hilo */
struct char_print_parms {
    char character;
    int count;
};

/* Función que ejecutarán los hilos */
void* char_print(void* parameters) {
    struct char_print_parms* p = (struct char_print_parms*) parameters;
    for (int i = 0; i < p->count; ++i) {
        putchar(p->character);
    }
    return NULL;
}

int main() {
    pthread_t thread1_id;
    pthread_t thread2_id;
    struct char_print_parms thread1_args;
    struct char_print_parms thread2_args;

    /* Crear un nuevo hilo para imprimir 30,000 'x' */
    thread1_args.character = 'x';
    thread1_args.count = 30000;
    pthread_create(&thread1_id, NULL, char_print, &thread1_args);

    /* Crear un nuevo hilo para imprimir 20,000 'o' */
    thread2_args.character = 'o';
    thread2_args.count = 20000;
    pthread_create(&thread2_id, NULL, char_print, &thread2_args);

    /* Esperar a que terminen los hilos */
    pthread_join(thread1_id, NULL);
    pthread_join(thread2_id, NULL);

    /* Retornar al finalizar */
    return 0;
}

