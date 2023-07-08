/*
    Written by William Sutherland for
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick

    Make using
        make problem2e

    Run using
        ./problem2e < test_case

    where test_case is a test case in the expected
        format (e.g. test_cases/2e-1.txt), for example:

        ./problem2e < test_cases/2e-1.txt
*/
#include "problem.h"
#include "map.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct problemE *p = readProblemE(stdin);

    printf("%d\n", minTimeDry(p->m, p->start, p->end, p->airports, p->numAirports));

    freeProblemE(p);

    return EXIT_SUCCESS;
}
