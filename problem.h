/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Header for module which contains problem 
        specification data structures and functions.
*/
#ifndef PROBLEM_H
#define PROBLEM_H

#include <stdio.h>
#include "map.h"

/* The struct for problemA */
struct problemA {
    struct point *testPoints;
    int numPoints;
    struct map *m;
};

struct problemD {
    struct map *m;
    struct point *start;
    struct point *end;
};

struct problemE {
    struct map *m;
    struct point *start;
    struct point *end;
    struct point *airports;
    int numAirports;
};

/* Reads Problem A from data */
struct problemA *readProblemA(FILE *f);

/* Reads Problem B from data */
struct map *readProblemB(FILE *f);

/* Reads Problem D from data */
struct problemD *readProblemD(FILE *f);

/* Reads Problem E from data */
struct problemE *readProblemE(FILE *f);

/* Reads a map from data */
struct map *readMap(FILE *f);

/* Frees the memory from the problems */
void freeProblemA(struct problemA *p);

void freeProblemD(struct problemD *p);

void freeProblemE(struct problemE *p);

#endif
