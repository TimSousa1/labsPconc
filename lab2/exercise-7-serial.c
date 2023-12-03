#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int *rand_num_array;

#define LIMIT 100000000

void *n_mult(void *arg){
    int n = *(int *) arg;
    printf("thread %d\n", n);
    long int c = 0;
    for(int i = 0 ; i < LIMIT; i++){
        if(rand_num_array[i] % n == 0)
        c++;
    }
    return (void *) c;
}
int main(){

    int mult_num[] = {2, 3, 5, 7, 11};
    ushort num_of_threads = sizeof(mult_num) / sizeof(*mult_num);
    printf("num_of_threads=%hu\n", num_of_threads);

    rand_num_array = calloc(LIMIT, sizeof(int));

    for(int i = 0 ; i < LIMIT; i++){
        rand_num_array[i] = random();
    }

    pthread_t thread_id[num_of_threads];
    void *ret = NULL;
    long results[num_of_threads];

    for (ushort i = 0; i < num_of_threads; i++){
        printf("creating thread %hu\n", mult_num[i]);
        pthread_create(&thread_id[i], NULL, n_mult, (void *) &mult_num[i]);
    }
    for (ushort i = 0; i < num_of_threads; i++){
        pthread_join(thread_id[i], &ret);
        results[i] = (long) ret;
    }
    for (ushort i = 0; i < num_of_threads; i++){
        printf("%ld ", results[i]);
    }
    printf("\n");
    exit(0);

}
