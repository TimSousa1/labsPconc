#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RAND_LIMIT 1000

//STEP 1

typedef struct {
    int pipe_read;
    int thread_id;

    int local_n_primes;
} thread_arg;


#define N_THREADS 4
/**
 * @brief funcao que rverifica se um n;umero é primo
 * 
 * @param value valor a verificar se é primo
 * @return int 1 se value for primo 0, se não for primo
 */
int verify_prime(int value){

    int divisor = 2;
    if(value < 4){
        return 1;
    }
    while (value % divisor != 0){
        divisor ++;
    }
    if (value == divisor){
        return 1;
    }else{
        return 0;
    }
}
/**
 * @brief código da thread que verificas se os numeros são primos
 * 
 * @param arg indice da thread.
 */
void *verify_primes_thread(void *arg){

    thread_arg *t = (thread_arg *) arg;

    int number;
    int local_count = 0;
    int local_n_primes = 0;

    while (1){
        // STEP 4
        if ( read(t->pipe_read, &number, sizeof(number)) < 1 ) break;
        local_count++;
        if(verify_prime(number) == 1){
            printf("\t\t%d is prime\n", number);
            local_n_primes ++;
        }else{
            //printf("\t\t%d is not prime\n", number);
        }
    } 
    printf("Thread %d found %d primes on %d processed randoms\n", t->thread_id, local_n_primes, local_count);
    t->local_n_primes = local_n_primes;

    pthread_exit(NULL);
}

/**
 * @brief programa principal que vários numeros aleatórios e verifica se são primos
 * 
 * @return int 
 */
int main(){

    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) return 1;

    // STEP 2

    int total_randoms;
    printf("Type how many random numbers should be verified ");
    if (scanf("%d", &total_randoms)!= 1 && total_randoms < 1){
        printf("invalid Number\n");
        exit(-1);
    }

    pthread_t t_id[N_THREADS];
    thread_arg args[N_THREADS];

    for(int i = 0 ; i < N_THREADS; i++){
        args[i].thread_id = i;
        args[i].pipe_read = pipe_fd[0];

        pthread_create(&t_id[i], NULL, verify_primes_thread, (void *) &args[i]);
    }

    int number;
    for(int i = 0; i < total_randoms; i++){
        number = random() % RAND_LIMIT;
        printf("%d\n", number);
        write(pipe_fd[1], &number, sizeof(number));
        //STEP 3
    }

    close(pipe_fd[1]);

    void *ret = NULL;
    int total_primes = 0;
    for(int i = 0; i < N_THREADS; i++){
        pthread_join(t_id[i], NULL);
        total_primes += args[i].local_n_primes;
    }

    printf("found %d primes\n", total_primes);

    close(pipe_fd[0]);
    exit(0);
}
