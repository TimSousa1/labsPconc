#include <stdio.h>
#include <pthread.h>  
#include <unistd.h> 
#include <stdlib.h>

void * thread_function(void *arg){
    int n = 0;
    long int r = random()%100;
    int arg_n = *(int *) arg;
    printf("Start Thread %lu\n", r);
    while(n <10){
        sleep(1);
        n++;
        printf("inside Thread %lu %d\n", r, n);
        printf("got arg %d\n", arg_n);
    }
    printf("End Thread %lu\n", r);
    return (void *)r;
}

int main(){
    char line[100];
    int n_threads;
    printf("How many threads: ");
    fgets(line, 100, stdin);
    sscanf(line, "%d", &n_threads);
    pthread_t threads[n_threads];

    int arg_int = 2;
    void *ret;

    long int i = 0;
    while(i < n_threads) {
        pthread_create(&threads[i], NULL,thread_function, &arg_int);
        i++;
    }
    for (i = 0; i < n_threads; i++){ 
        pthread_join(threads[i], &ret);
        printf("thread %li\n", (long) ret);
    }
    exit(0);
}
