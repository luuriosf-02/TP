#include <stdio.h>
 #include <stdlib.h>
 
 int main() {

 const char* server_name = getenv("SERVER_NAME");
 
 if (server_name == NULL) {
 server_name = "servidor.de.MarayLujan.com";
 }

 printf("Accediendo al servidor: %s\n", server_name);
 
 return 0;
 }
