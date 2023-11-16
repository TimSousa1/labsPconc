#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcpuid.h>

#include "word.h"

int main(){
    char *filename = "lusiadas-words.txt";

    WordArray *array  = create_array(filename);
    if (!array) return -1;

    struct cpu_raw_data_t raw;
    struct cpu_id_t data;

    cpuid_get_raw_data(&raw);
    cpu_identify(&raw, &data);
    printf("No. of Physical Core(s) : %d\n", data.num_cores);
    return 0;
    print_array(array);

    free_array(array);
    return 0;
}
