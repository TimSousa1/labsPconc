typedef struct _wordArray {
    char **words;
    uint size;
} WordArray;

void print_array(WordArray *array);
void free_array(WordArray *array);
// returns NULL if allocation fails
WordArray *create_array(char *filename);
