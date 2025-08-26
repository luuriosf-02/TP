#include <stdio.h>
int f ()
{
 	return 3;
} 

int main() {
    int x = f();
    printf("La función f() devolvió: %d\n", x);
    return 0;
}
