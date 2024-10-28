#include <stdlib.h>
#include <stdio.h>
#include "dep.h"

void exit_on_invalid_args(int given, int expected) {
    printf("Missing required argument! Given %d, but expected %d", given, expected);
    exit(2);
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        exit_on_invalid_args(argc, 2);
    
    print_hello(argv[1]);
    return 0;
}