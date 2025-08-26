#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

volatile int thread_flag; // Volatile para que no se optimice el acceso
pthread_mutex_t thread_flag_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_flag_cond = PTHREAD_COND_INITIALIZER;

/* Inicializa la bandera y el mutex */
void initialize_flag()
{
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = 0;
    pthread_mutex_unlock(&thread_flag_mutex);
}

/* Función que hace el trabajo */
void do_work()
{
    printf("Trabajando...\n");
    usleep(100000); // Simula trabajo de 0.1s
}

/* Thread que llama a do_work mientras la bandera esté activada */
void* thread_function(void* thread_arg)
{
    while (1) {
        pthread_mutex_lock(&thread_flag_mutex);
        while (thread_flag == 0) {
            // Espera a que la bandera cambie
            pthread_cond_wait(&thread_flag_cond, &thread_flag_mutex);
        }
        pthread_mutex_unlock(&thread_flag_mutex);

        // Ejecuta el trabajo
        do_work();
    }
    return NULL;
}

/* Establece el valor de la bandera */
void set_thread_flag(int flag_value)
{
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_cond_signal(&thread_flag_cond); // Despierta al thread si estaba esperando
    pthread_mutex_unlock(&thread_flag_mutex);
}

/* Ejemplo de main */
int main()
{
    pthread_t thread;
    initialize_flag();

    pthread_create(&thread, NULL, thread_function, NULL);

    sleep(1);
    printf("Activando la bandera\n");
    set_thread_flag(1);

    sleep(2);
    printf("Desactivando la bandera\n");
    set_thread_flag(0);

    sleep(1);
    return 0;
}

