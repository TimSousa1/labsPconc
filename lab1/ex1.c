#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

char **readWords(FILE *file, int *elements) {
    char **wordArray;
    char *word;
    int count = 0;

    while (fscanf(file, "%s ", word) == 1) {
        count++;
    }
    if (!count) {
        printf("There are no words in the file!\n");
        exit(EXIT_SUCCESS);
    }

    wordArray = (char **) malloc(count * sizeof(char *));

    rewind(file);
    count = 0;
    while (fscanf(file, "%s ", word) == 1) {
        wordArray[count] = (char *) malloc(strlen(word) * sizeof(char));
        strcpy(wordArray[count], word);
        count++;
    }

    *elements = count;

    return wordArray;
}

int alphabeticUp(char *first, char *second) {

    if (second[0] < first[0]) return 1;
    else return -1;

}

void quickSort(char **wordArray, int l, int r) {
    
    int i, j;
    char *pivot, *tmp;

    if (l >= r || l < 0) return;
    pivot = wordArray;
    /* Temporary pivot index */
    i = l - 1;
    for (j = l ; j < r; j++) {

        /* If the current element is less than or equal to the pivot */
        if (strcasecmp(wordArray[j], pivot) > 0) {
            
            /* Move the temporary pivot index forward */
            i = i + 1;

            /* Swap the current element with the element at the temporary pivot index */
            tmp = wordArray[i];
            wordArray[i] = wordArray[j];
            wordArray[j] = tmp;
        }
    }
    /* Move the pivot element to the correct pivot position (between the smaller and larger elements) */
    i = i + 1;
    tmp = wordArray[r];
    wordArray[r] = wordArray[i];
    wordArray[i] = tmp;

    quickSort(wordArray, l, i - 1); /* Left side of pivot */
    quickSort(wordArray, i + 1, r); /* Right side of pivot */

    return;
}

void printWords(char **wordArray, unsigned long int numOfWords) {

    for (int i = 0; i < numOfWords; i++) {
        printf("%s\n", wordArray[i]);
    }
    printf("Number of words: %lu\n", numOfWords);

}

int main(int argc, char **argv) {

    FILE* words = fopen("lusiadas-words.txt", "r");
    if (!words) {
        fprintf(stderr, "There is no file named lusiadas-words.txt!\n");
        exit(EXIT_FAILURE);
    }
    
    int elements;
    char **allWords = readWords(words, &elements);

    //printWords(allWords, elements);

    quickSort(allWords, 0, elements-1);

    printWords(allWords, elements);

    return 0;
}
