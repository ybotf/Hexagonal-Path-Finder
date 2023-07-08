/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Make using
        make problem2b
    
    Run using
        ./problem2b < test_case
    
    where test_case is a test case in the expected
        format (e.g. test_cases/2b-1.txt), for example:
    
        ./problem2b < test_cases/2b-1.txt
*/
#include "map.h"
#include "problem.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct map *m = readProblemB(stdin);

    printf("%d\n", mapValue(m));

    freeMap(m);

    return EXIT_SUCCESS;
}
