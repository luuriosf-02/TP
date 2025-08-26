#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* El nombre de este programa. */
const char* program_name;

/*
 * Muestra información de uso para este programa en un flujo de datos (normalmente
 * stdout o stderr) y termina el programa con un código de salida (exit_code).
 * No regresa de la función.
 */
void print_usage (FILE* stream, int exit_code)
{
    fprintf (stream, "Uso: %s opciones [ archivo_de_entrada .... ]\n", program_name);
    fprintf (stream,
            " -h --help    Muestra esta información de uso.\n"
            " -o --output  <nombre_de_archivo>  Escribe la salida en un archivo.\n"
            " -v --verbose Muestra mensajes detallados.\n");
    exit (exit_code);
}

/*
 * Punto de entrada principal del programa.
 * ARGC contiene el número de elementos de la lista de argumentos.
 * ARGV es un array de punteros a esos elementos.
 */
int main (int argc, char* argv[])
{
    int next_option;

    /* Una cadena que lista las letras válidas de las opciones cortas. */
    const char* const short_options = "ho:v";

    /* Un array que describe las opciones largas válidas. */
    const struct option long_options[] = {
        { "help",    0, NULL, 'h' },
        { "output",  1, NULL, 'o' },
        { "verbose", 0, NULL, 'v' },
        { NULL,      0, NULL, 0 }  /* Requerido al final del array. */
    };

    /* El nombre del archivo para la salida del programa, o NULL para la salida estándar. */
    const char* output_filename = NULL;

    /* Si se debe mostrar mensajes detallados. */
    int verbose = 0;

    /*
     * Almacena el nombre del programa, para incluirlo en los mensajes.
     * El nombre se encuentra en argv[0].
     */
    program_name = argv[0];

    do {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);

        switch (next_option)
        {
            case 'h':   /* -h o --help */
                /* El usuario ha solicitado información de uso. Imprímela en la salida
                 * estándar y sal con el código de salida cero (terminación normal).
                 */
                print_usage (stdout, 0);
                break; // Se agregó el 'break'

            case 'o':   /* -o o --output */
                /* Esta opción toma un argumento, el nombre del archivo de salida. */
                output_filename = optarg;
                break;

            case 'v':   /* -v o --verbose */
                verbose = 1;
                break;

            case '?':   /* El usuario especificó una opción inválida. */
                /* Muestra información de uso en el error estándar y sal con el
                 * código de salida uno (indicando terminación anormal).
                 */
                print_usage (stderr, 1);
                break; // Se agregó el 'break'

            case -1:    /* Terminado con las opciones. */
                break;

            default:    /* Algo más: inesperado. */
                abort();
        }
    } while (next_option != -1);

    /*
     * Terminó el procesamiento de opciones. La variable 'optind' apunta al primer
     * argumento que no es una opción. Para fines de demostración, los imprimimos
     * si se especificó la opción 'verbose'.
     */
    if (verbose) {
        int i;
        for (i = optind; i < argc; ++i) {
            printf ("Argumento: %s\n", argv[i]);
        }
    }
    
    /* El programa principal continuaría aquí. */
    return 0;
}
