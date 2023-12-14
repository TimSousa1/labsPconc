#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_SIZE 100
#define N_THREADS 4

char **pipe_fd[2];

void *thread_string_size(void *arg) {
	char *string;
	while(1) {
		if (read(pipe_fd[0], string, MAX_SIZE) < 1) pthread_exit(NULL);
		printf("String: %s has %d characters", string, strlen(string));
	}
}

int main(int argc, char** argv) {

	char string[MAX_SIZE];
	pthread_t thread_ids[N_THREADS];

	for (int i = 0; i < N_THREADS; i++) {
		pthread_create(&thread_ids[i], NULL, thread_string_size, NULL);
	}

	while(fgets(string, MAX_SIZE, stdin)) {
		write(pipe_fd[1], string, MAX_SIZE);

	}
}

