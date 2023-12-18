#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * @brief program that creates a pipe, writes values to a it and reads them
 * 
 */
typedef struct {
    int pipe_read;
    int thread_id;
} thread_arg;

void *thread_read_values(void *arg){
    thread_arg val = *(thread_arg *) arg;
    int n;
    while (1){
        read(val.pipe_read, &n, sizeof(n));
        printf("\tthread %d read %d\n", val.thread_id, n);
    }
    return NULL;
}


int main(){
    // tow files descriptors used to write. read on the pipe
    int pipe_fd[2];

    // initialization of the pipe
    if ( pipe(pipe_fd) != 0 ){
        printf("error creating the pipe");
        exit(-1);
    }

#define N_THREADS 4
    pthread_t threads[N_THREADS];
    thread_arg arg[N_THREADS];

    for (int i = 0; i < N_THREADS; i++){
        arg[i].pipe_read = pipe_fd[0];
        arg[i].thread_id = i;
        pthread_create(&threads[i], NULL, thread_read_values, (void *) &arg[i]);
    }

    int n = 0;
    int read_value;
    // infinite loop that in each iteration writes a number to the pipe and read it afterwards
    while(1){
        n++;

        printf("going to write %d into pipe_fd[1]\n", n);
        sleep(1);
        
        write(pipe_fd[1], &n, sizeof(n));
        // pipe_fd[1] correpsonded to en entry of data in the pipe
        // &n is apointer to the data to be written
        // sizeof(n) is the ammount of data to write in the pipe

        // read(pipe_fd[0], &read_value, sizeof(read_value));
        // pipe_fd[0] correpsonded to the exit of data in the pipe
        // &n is apointer to variable that will hold the read data
        // sizeof(n) is the ammount of data to be read from the pipe
        // printf("\tjust read %d from pipe_fd[0]\n",read_value);

    }

}
