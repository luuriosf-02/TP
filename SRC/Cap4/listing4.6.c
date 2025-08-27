#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Número de cuentas */
#define NUM_ACCOUNTS 10

/* Array de saldos */
float* account_balances;

/* Mutex para proteger transferencias */
pthread_mutex_t balance_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Transferir DOLLARS de FROM_ACCT a TO_ACCT.
   Retorna 0 si la transacción tuvo éxito, 1 si fondos insuficientes. */
int process_transaction(int from_acct, int to_acct, float dollars) {
    int result = 0;

    /* Comenzar sección crítica */
    pthread_mutex_lock(&balance_mutex);

    /* Verificar fondos */
    if (account_balances[from_acct] < dollars) {
        result = 1; // no hay suficiente saldo
    } else {
        /* Realizar transferencia */
        account_balances[to_acct]   += dollars;
        account_balances[from_acct] -= dollars;
    }

    /* Fin de sección crítica */
    pthread_mutex_unlock(&balance_mutex);

    return result;
}

int main() {
    /* Reservar memoria para cuentas e inicializar */
    account_balances = calloc(NUM_ACCOUNTS, sizeof(float));
    if (!account_balances) {
        perror("Error al asignar memoria");
        return 1;
    }

    /* Ejemplo: inicializamos cuentas */
    account_balances[0] = 1000.0;
    account_balances[1] = 500.0;

    /* Intentamos transferir */
    if (process_transaction(0, 1, 200.0) == 0) {
        printf("Transferencia exitosa.\n");
    } else {
        printf("Fondos insuficientes.\n");
    }

    printf("Cuenta 0: %.2f\n", account_balances[0]);
    printf("Cuenta 1: %.2f\n", account_balances[1]);

    /* Liberar memoria */
    free(account_balances);

    return 0;
}

