#include <stdio.h>
#include <pthread.h>  
#include <unistd.h> 
#include <stdlib.h>

void * thread_function(void * arg){
    int n = 0;
    long int r = random()%100;
    printf("Start Thread %lu\n", r);
    while(n <10){
        sleep(1);
        n++;
        printf("inside Thread %lu %d\n", r, n);
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
    pthread_t thread_id;

    long int i = 0;
    pthread_t threads[n_threads];
    while( i < n_threads) {
        pthread_create(&threads[i], NULL,thread_function, NULL);
        //threads[i] = thread_id;
		i++;
    };
	void *thread_ret;
	long int ret;
    for (i = 0; i < n_threads; i++) {
		pthread_join(threads[i], &thread_ret);
		ret = (long int) thread_ret;
		printf("%li\n", (int) ret);
    }    
    exit(0);
}
