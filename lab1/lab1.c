#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(){
    char line[100];
    int n;
    printf("What is your student number?\n");
    fgets(line, 100, stdin);
    if (sscanf(line, "%d", &n) != 1){
        printf("Invalid Number!\n");
        exit(-1);
    }
    srandom(n);
    printf("Your random number is %li!\n", random());   
    exit(0);
}
