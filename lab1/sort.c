#include <stdlib.h>
#include <stdbool.h>

void sort(void *a, void *b){
    char *as = (char *) a;
    char *bs = (char *) b;

    if (as[0] < bs[0])
}

void merge(void **array, uint start, uint middle, uint end, bool (*sort)(void*, void*)){
    void *tmp[end - start +1];
    uint tmp_index = 0;

    uint i = start, j = middle+1;
    bool i_end, j_end;

    while(i <= middle && j <= end){
        if (sort(array[i], array[j])) tmp[tmp_index] = array[i++];
        else tmp[tmp_index] = array[j++];
        tmp_index++;
    }
    while (i <= middle) tmp[tmp_index] = array[i++];
    while (j <= middle) tmp[tmp_index] = array[j++];
    while(start <= end) array[end--] = tmp[tmp_index--];
}

void mergesort(void **array, uint start, uint end, bool (*sort)(void*, void*)){
    if (end < start) return;

    uint middle;
    middle = (start + end)/2;

    mergesort(array, start, middle, sort);
    mergesort(array, middle+1, end, sort);
    merge(array, start, middle, end, sort);
}
