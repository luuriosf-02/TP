#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
void* thread_function (void* arg)
{
 fprintf (stderr, "child thread pid is %d\n", (int) getpid ());
 /* Spin forever. */
 while (1);
 return NULL;
}
int main ()
{
 pthread_t thread;
 fprintf (stderr, "main thread pid is %d\n", (int) getpid ());
 pthread_create (&thread, NULL, &thread_function, NULL);
 /* Spin forever. */
 while (1);
 return 0;
} 

/* //Run the program in the background, and then invoke ps x to display your running processes. Don't
forget to kill the thread-pid program afterward—it consumes lots of CPU doing nothing. Here's
what the output might look like:
% cc thread-pid.c -o thread-pid -lpthread
% ./thread-pid &
[1] 14608
main thread pid is 14608
child thread pid is 14610
% ps x
 PID TTY STAT TIME COMMAND
14042 pts/9 S 0:00 bash
14608 pts/9 R 0:01 ./thread-pid
14609 pts/9 S 0:00 ./thread-pid
14610 pts/9 R 0:01 ./thread-pid
14611 pts/9 R 0:00 ps x
% kill 14608
[1]+ Terminated ./thread-pid  */