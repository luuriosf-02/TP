
#include <stdio.h>
#include <tiffio.h>   

int main(int argc, char* argv[]) {
    const char* filename;

    if (argc < 2) {
        filename = "SRC/Cap2/prueba2.9lp.tiff"; 
        printf(" No se paso ningun archivo, usando por default: %s\n", filename);
    } else {
        filename = argv[1];
        printf("Abriendo documento, espere... %s\n", filename);
    }


    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        fprintf(stderr, " Error: no se pudo abrir el archivo TIFF %s\n", filename);
        return 1;
    }

    printf("Archivo libtiff %s abierto correctamente \n", filename);

   
    TIFFClose(tif);

    printf(" Procesamiento finalizado correctamente\n");
    return 0;
}

