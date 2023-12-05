#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include "utils.h"

/* program that counts unique words per letter*/

word_info *words_lists[26];
int n_total_words;
char **word_array;

void *thread_find_unique_words_letters(void *chr){
    char c = *(char *) chr;
    printf("got char %c\n", c);

    word_info *list_aux = words_lists[c - 'a'];

    // iterates through all words in the array
    for (int i = 0; i < n_total_words; i++){

        // verifies if the word starts with a letter
        if (isalpha(word_array[i][0])){
            if (word_array[i][0] != c) continue;

            // finds occurrences of the word in it's list
            for(list_aux = words_lists[c - 'a']; list_aux->next != NULL;){

                // if the word is found, increment the counter and stop searching
                if(strcmp(word_array[i], list_aux->next->word) == 0){
                    list_aux->next->count++;
                    break;
                }
                list_aux=list_aux->next;
            }
            // if the end of the list is reached..
            if(list_aux->next == NULL){

                // insert new word in the list
                list_aux->next = malloc(sizeof(word_info));
                list_aux->next->next = NULL;
                list_aux->next->count = 1;
                list_aux->next->word = word_array[i];
            }

        }
    }
    return NULL;
}


void find_unique_words_letters(){

    word_info *list_aux;

    // iterates through all words in the array
    for (int i = 0; i< n_total_words;i++){

        //verifies if the word starts with a letter
        if (isalpha(word_array[i][0])){
            char current_letter = word_array[i][0];
            list_aux = words_lists[current_letter - 'a'];

            // finds occurrences of the word in it's list
            while(list_aux->next != NULL){

                // if the word is found, increment the counter and stop searching
                if( strcmp(word_array[i], list_aux->next->word) == 0){
                    list_aux->next->count++;
                    break;
                }
                list_aux=list_aux->next;
            }
            // if the end of the list is reached..
            if(list_aux->next == NULL){

                // insert new word in the list
                list_aux->next = malloc(sizeof(word_info));
                list_aux->next->next = NULL;
                list_aux->next->count = 1;
                list_aux->next->word = word_array[i];
            }

        }
    }
}



void print_more_freq_words_letters(){
    word_info * list_aux;
    char more_freq_word[100];

    // iterates all list of words
    for (int c ='a'; c <= 'z'; c++){

        // initializes more frequentg word and its values
        int more_freq_word_count = 0;
        strcpy(more_freq_word, "");

        // search on the suitable list the more frequenty word
        list_aux = words_lists[c-'a']->next;
        while (list_aux!=NULL){
            if (list_aux ->count > more_freq_word_count){
                more_freq_word_count = list_aux ->count;
                strcpy(more_freq_word, list_aux ->word);
            }
            list_aux  = list_aux ->next;
        }
        printf("%c %s %d\n", c, more_freq_word, more_freq_word_count);
    }

}

void read_word_array(){

    FILE * fp;

    char linha[100];

    fp = fopen("./lusiadas-words.txt", "r");
    while (fgets(linha, 100, fp)!= NULL){
        n_total_words++;
    }
    printf("nwords - %d\n", n_total_words);
    fclose(fp);

    // creates one list per letter
    for (int c ='a'; c <= 'z'; c++){
        words_lists[c-'a']=  malloc(sizeof(word_info));
        words_lists[c-'a']->next = NULL;
    }

    // creates the array of words
    word_array = calloc(sizeof(char*), n_total_words);

    // inserts all words in the array
    fp = fopen("./lusiadas-words.txt", "r");  
    for (int i = 0; i < n_total_words; i++){
        word_array[i] = calloc(sizeof(char), MAX_LENGTH);
        fgets(word_array[i], 100, fp);
        strlower (word_array[i]);
        if (word_array[i][strlen(word_array[i])-1] == '\n'){
            word_array[i][strlen(word_array[i])-1] = '\0';
        }
    }
}


void delete_word_array(){
    for (int i = 0; i< n_total_words; i++){
        free(word_array[i]);
    }
    free(word_array);
}

void delete_unique_words_lists(){
    word_info *aux_list;
    for (int c ='a'; c <= 'z'; c++){
        while(words_lists[c-'a'] != NULL){
            aux_list = words_lists[c-'a'];
            words_lists[c-'a'] = words_lists[c-'a']->next;
            free(aux_list);
        }
    }
}

int main(){

    struct timespec start_time_total, end_time_total;
    struct timespec start_time_seq_1, end_time_seq_1;
    struct timespec start_time_seq_2, end_time_seq_2;
    struct timespec start_time_par_1, end_time_par_1;
    struct timespec start_time_par_2, end_time_par_2;


    clock_gettime(CLOCK_MONOTONIC, &start_time_total);

    // Read of words into word_array
    clock_gettime(CLOCK_MONOTONIC, &start_time_seq_1);
    read_word_array();
    clock_gettime(CLOCK_MONOTONIC, &end_time_seq_1);


    // creation of lists of uniq words
    clock_gettime(CLOCK_MONOTONIC, &start_time_par_1);

#ifdef MULTI_THREADING
    
#define N_THREADS 26

    pthread_t threads[N_THREADS];
    int chars_array[N_THREADS];
    for (int i = 0; i < N_THREADS; i++){
        chars_array[i] = i + 'a';
        printf("creating thread %i\n", i);
        pthread_create(&threads[i], NULL, thread_find_unique_words_letters, (void *) &chars_array[i]);
    }

    for (int i = 0; i < N_THREADS; i++) {
        printf("waiting for thread %i\n", i);
        pthread_join(threads[i], NULL);
    }

#else
    find_unique_words_letters();
#endif

    clock_gettime(CLOCK_MONOTONIC, &end_time_par_1);

    // printing of more frequent words per letter
    clock_gettime(CLOCK_MONOTONIC, &start_time_par_2);
    print_more_freq_words_letters();
    clock_gettime(CLOCK_MONOTONIC, &end_time_par_2);

    clock_gettime(CLOCK_MONOTONIC, &start_time_seq_2);
    delete_word_array();
    delete_unique_words_lists();
    clock_gettime(CLOCK_MONOTONIC, &end_time_seq_2);

    clock_gettime(CLOCK_MONOTONIC, &end_time_total);
    struct timespec seq_1_time = diff_timespec(&end_time_seq_1, &start_time_seq_1);
    struct timespec seq_2_time = diff_timespec(&end_time_seq_2, &start_time_seq_2);
    struct timespec par_1_time = diff_timespec(&end_time_par_1, &start_time_par_1);
    struct timespec par_2_time = diff_timespec(&end_time_par_2, &start_time_par_2);
    struct timespec total_time = diff_timespec(&end_time_total, &start_time_total);
    printf("\tseq 1\t %10jd.%09ld\n",
            seq_1_time.tv_sec, seq_1_time.tv_nsec);
    printf("\tseq 2\t %10jd.%09ld\n",
            seq_2_time.tv_sec, seq_2_time.tv_nsec);
    printf("\tpar 1 \t %10jd.%09ld\n",
            par_1_time.tv_sec, par_1_time.tv_nsec);
    printf("\tpar 2 \t %10jd.%09ld\n",
            par_2_time.tv_sec, par_2_time.tv_nsec);
    printf("total \t %10jd.%09ld\n",
            total_time.tv_sec, total_time.tv_nsec);


    exit(0);
}

/*
seq 1            6.417117223
seq 2            0.192842990
par 1           14.075368674
par 2            0.000234675
total           20.685563956
*/

/*                          pc1      |     pc2
 * cpus                      8       |      4
 * total time           6.679409384  |  20.685563956
 * parallel time                     | 
 * sequential time                   |
 * Fpar                              |
 * Fser                              |
 * Maxspeedup                        |
 *
 *
 *
 */
