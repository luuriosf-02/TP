
#include <stdio.h>
#include <tiffio.h>   

int main(int argc, char* argv[]) {
    const char* filename;

    if (argc < 2) {
        filename = "SRC/Cap2/prueba2.9lp.tiff"; 
        printf(" No se pasó ningún archivo. Usando por defecto: %s\n", filename);
    } else {
        filename = argv[1];
        printf("Intentando abrir: %s\n", filename);
    }

    // Intentar abrir TIFF
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        fprintf(stderr, " Error: no se pudo abrir el archivo TIFF %s\n", filename);
        return 1;
    }

    printf("Archivo %s abierto correctamente con libtiff.\n", filename);

   
    TIFFClose(tif);

    printf(" Procesamiento finalizado sin errores.\n");
    return 0;
}

