/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Implementation for module which contains problem 
        specification data structures and functions.
*/
/*
It is highly recommended that you do not change these files as it may make your tests
not run
*/

#include "problem.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define INT_CHAR_SIZE 12

struct problemA *readProblemA(FILE *f) {
    struct problemA *problem = (struct problemA *) malloc(sizeof(struct problemA));
    assert(problem);

    // First line of input represents the number of points we are testing
    assert(scanf("%d", &(problem->numPoints)) == 1);
    problem->testPoints = (struct point *) malloc(problem->numPoints * sizeof(struct point));
    assert(problem->testPoints);

    // Scans all the points
    for (int i = 0; i < problem->numPoints; i++) {
        assert(scanf("%d %d", &(problem->testPoints[i].x), &(problem->testPoints[i].y)) == 2);
    }

    int height; int width;
    assert(scanf("%d %d", &width, &height) == 2);
    problem->m = newMap(height, width);

    return problem;
}

struct map *readProblemB(FILE *f) {
    return readMap(f);
}

struct problemD *readProblemD(FILE *f) {
    struct problemD *problem = (struct problemD *) malloc(sizeof(struct problemD));
    assert(problem);
    problem->start = (struct point *) malloc(sizeof(struct point));
    assert(problem->start);
    problem->end = (struct point *) malloc(sizeof(struct point));
    assert(problem->end);

    assert(scanf("%d %d", &(problem->start->x), &(problem->start->y)) == 2);
    assert(scanf("%d %d", &(problem->end->x), &(problem->end->y)) == 2);

    problem->m = readMap(f);

    return problem;
}

struct problemE *readProblemE(FILE *f) {
    struct problemE *problem = (struct problemE *) malloc(sizeof(struct problemE));
    assert(problem);
    problem->start = (struct point *) malloc(sizeof(struct point));
    assert(problem->start);
    problem->end = (struct point *) malloc(sizeof(struct point));
    assert(problem->end);

    assert(scanf("%d %d", &(problem->start->x), &(problem->start->y)) == 2);
    assert(scanf("%d %d", &(problem->end->x), &(problem->end->y)) == 2);
    assert(scanf("%d", &(problem->numAirports)) == 1);

    problem->airports = (struct point *) malloc(problem->numAirports * sizeof(struct point));
    assert(problem->airports);

    for (int i = 0; i < problem->numAirports; i++) {
        assert(scanf("%d %d", &(problem->airports[i].x), &(problem->airports[i].y)) == 2);
    }

    problem->m = readMap(f);

    return problem;
}

struct map *readMap(FILE *f) {
    int height; int width;
    assert(scanf("%d %d ", &width, &height) == 2);
    struct map *m = newMap(height, width);

    size_t lineLength = 0;
    char *line = NULL;
    int i = 0;

    while (getline(&line, &lineLength, f) > 0) {
        char *curr = line;
        int read;
        int j = 0;
        int currHeight = i;
        int nextVal;
        while (sscanf(curr, "%d%n", &nextVal, &read) == 1) {
            m->points[j][currHeight] = nextVal;
            j++;
            curr += read;
            currHeight = m->height - currHeight - 1;
        }

        i++;
    }

    if(line){
        free(line);
    }

    return m;
}

void freeProblemA(struct problemA *p) {
    freeMap(p->m);
    free(p->testPoints);
    free(p);
}

void freeProblemD(struct problemD *p) {
    freeMap(p->m);
    free(p->start);
    free(p->end);
    free(p);
}

void freeProblemE(struct problemE *p) {
    freeMap(p->m);
    free(p->airports);
    free(p->start);
    free(p->end);
    free(p);
}
