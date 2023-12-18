#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define N_THREADS 4
#define COUNT 100

typedef struct {
    pthread_t thread_id;
    int pipe_read;
} thread_args;


void *thread_count_chars(void *arg){
    thread_args *t = (thread_args*) arg;

    char line[COUNT];
    int n_chars;
    while (1){
        read(t->pipe_read, &line, sizeof(line));
        n_chars = strlen(line) -1;
        printf("%d\n", n_chars);
    }
    return NULL;
}

int main(){
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) return 1;

    pthread_t threads[N_THREADS];
    thread_args args[N_THREADS];

    for (int i = 0; i < N_THREADS; i++){
        args[i].thread_id = i;
        args[i].pipe_read = pipe_fd[0];
        pthread_create(&threads[i], NULL, thread_count_chars, (void *) args);
    }

    char line[COUNT];
    while (1){
        fgets(line, sizeof(line), stdin);
        write(pipe_fd[1], line, sizeof(line));
    }

    return 0;
}
