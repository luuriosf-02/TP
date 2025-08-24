#include <stdio.h>  
#include "Listing1.3.hpp"  
#include <stdlib.h>
int main (int argc, char **argv)  
{ 
  int i; 
  i = atoi (argv[1]);  
  printf ("The reciprocal of %d is %g\n", i, reciprocal (i));  
  return 0;  
} 
