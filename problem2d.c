/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Make using
        make problem2d
    
    Run using
        ./problem2d < test_case
    
    where test_case is a test case in the expected
        format (e.g. test_cases/2d-1.txt), for example:
    
        ./problem2d < test_cases/2d-1.txt
*/
#include "map.h"
#include "problem.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct problemD *p = readProblemD(stdin);

    printf("%d\n", minTime(p->m, p->start, p->end));

    freeProblemD(p);

    return EXIT_SUCCESS;
}
