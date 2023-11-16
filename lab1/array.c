#include <stdio.h>
#include <stdlib.h>
#include "word.h"

void print_array(WordArray *array){

    printf("[I] array is of size %i\n", array->size);
    printf("[I] printing words present in array...\n");

    for (int i = 0; i < array->size; i++) printf("[P] %s ", array->words[i]);
}

void free_array(WordArray *array){
    printf("[I] freeing array with %i words\n", array->size);

    for (int i = 0; i < array->size; i++) free(array->words[i]);

    free(array->words);
    free(array);
}

// returns NULL if allocation fails
WordArray *create_array(char *filename){
    FILE *file = fopen(filename, "r");

    char line[100];
    int word_count = 0;

    while (fgets(line, sizeof(line), file)) word_count++;
    printf("[I] found %i words\n", word_count);

    WordArray *array = (WordArray*) malloc (sizeof(*array));
    if (!array) return NULL;

    char **words = (char **) calloc(word_count, sizeof(char *));
    if (!words) {
        free(array);
        return NULL;
    }

    rewind(file);

    uint current_index = 0;
    while(fgets(line, sizeof(line), file)){
        words[current_index] = (char *) malloc ((strlen(line)+1) * sizeof(char));
        if (!words[current_index]) {
            fprintf(stderr, "[E] Couldn't allocate memory at index %i", current_index);

            array->size = current_index;
            free_array(array);

            return NULL;
        }
        strcpy(words[current_index], line);
        current_index++;
    }

    array->words = words;
    array->size = word_count;

    return array;
}
