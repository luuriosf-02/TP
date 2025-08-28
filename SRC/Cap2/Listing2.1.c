#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("El nombre de este programa es: '%s'.\n", argv[0]);
    printf("Este programa fue ejecutado con %d argumentos.\n", argc - 1);

    
    if (argc > 1) {
        int i;
        printf("Los argumentos son:\n");
        for (i = 1; i < argc; ++i) {
            printf("%s\n", argv[i]);
        }
    }

    return 0;
}
