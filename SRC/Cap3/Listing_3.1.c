#include <stdio.h>
#include <unistd.h>


int main ( )
{

 printf ("El ID del proceso es: %d\n", (int) getpid ()) ;
 printf ("El proceso padre es: %d\n", (int) getppid ()) ;
 
 return 0 ;
 
} 

