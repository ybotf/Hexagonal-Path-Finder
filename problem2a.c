/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Make using
        make problem2a
    
    Run using
        ./problem2a < test_case
    
    where test_case is a test case in the expected
        format (e.g. test_cases/2a-1.txt), for example:
    
        ./problem2a < test_cases/2a-1.txt
*/
#include "map.h"
#include "problem.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    struct problemA *problem = readProblemA(stdin);

    for (int i = 0; i < problem->numPoints; i++) {
        struct point *adjPoints = getAdjacentPoints(problem->m, &(problem->testPoints[i]));

        /* Use a temporary pointer to iterate through
            points. */
        struct point *adjPoint = adjPoints;
        while (adjPoint->x != -1) {
            printf("(%d, %d) ", adjPoint->x, adjPoint->y);
            adjPoint = adjPoint + 1;
        }

        if(adjPoints){
            free(adjPoints);
        }

        printf("\n");
    }

    freeProblemA(problem);

    return EXIT_SUCCESS;
}
