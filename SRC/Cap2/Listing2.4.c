#include <stdio.h>
 #include <stdlib.h>
 
 int main() {
 // Obtener el valor de la variable de entorno SERVER_NAME
 const char* server_name = getenv("SERVER_NAME");
 
 // Si SERVER_NAME no está definida, usar valor por defecto
 if (server_name == NULL) {
 server_name = "server.my-company.com";
 }
 
 // Imprimir el servidor que se va a acceder
 printf("Accessing server: %s\n", server_name);
 
 // Aquí iría la lógica para acceder al servidor...
 
 return 0;
 }
