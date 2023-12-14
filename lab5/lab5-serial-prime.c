#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//STEP 1
int pipe_fd[2];


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
    while (value%divisor != 0){
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
void * verify_primes_thread(void * arg){

    int int_arg = (int) arg;
    int number;
    int local_count = 0;
    int local_n_primes = 0;
    while (1){
        // STEP 4
		if (read(pipe_fd[0], &number, sizeof(number)) < 1) {
			int *ret = malloc(sizeof(int)); 
			*ret = local_n_primes;
			printf("Thread %d found %d primes on %d processed randoms\n", int_arg, local_n_primes, local_count);
			pthread_exit((void *)ret);
		}
		local_count++;
        if(verify_prime(number) == 1){
            printf("\t\t%d is prime\n", number);
            local_n_primes ++;
        }else{
            printf("\t\t%d is not prime\n", number);
        }
    } 
}

/**
 * @brief programa principal que vários numeros aleatórios e verifica se são primos
 * 
 * @return int 
 */
int main(){

    // STEP 2
	pipe(pipe_fd);
    int total_randoms;
    printf("Type how many random numbers should be verified ");
    if (scanf("%d", &total_randoms)!= 1 && total_randoms<1){
        printf("invalid Number\n");
        exit(-1);
    }


    // ciclo para criar as várias threads que irão verificar os numeros
    pthread_t t_id[N_THREADS];

    for(int i = 0 ; i < N_THREADS; i++){
        pthread_create(&t_id[i], NULL, verify_primes_thread, (void *)i);
    }


    int numbers[total_randoms];
    int n_primes = 0;
    for(int i = 0; i< total_randoms; i++){
        numbers[i] = random() % 50;
		//printf("%d\n", numbers[i]);
        //STEP 3
		write(pipe_fd[1], &numbers[i], sizeof(numbers[i]));
    }
	close(pipe_fd[1]);
	void *ret;
	for (int i = 0 ; i < N_THREADS; i++){
		pthread_join(t_id[i], &ret);
		n_primes += *(int *) ret;
	}
    printf("%d primes in %d randoms\n", n_primes, total_randoms);

/*
    ciclo que esperara a terminação das threads
    for(int i = 0 ; i < N_THREADS; i++){
        pthread_join(t_id[i],  NULL);
    }
*/
    exit(0);
}
