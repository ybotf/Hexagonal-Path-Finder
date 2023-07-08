/*
    Written by William Sutherland for
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick

    Implementation for module which contains map-related
        data structures and functions.

    Functions in the task description to implement can
        be found here.

    Code implemented by <toby>
*/
#include "map.h"
#include "math.h"
#include "stack.h"
#include "pq.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define INF -999999999

struct map *newMap(int height, int width)
{
    struct map *m = (struct map *)malloc(sizeof(struct map));
    assert(m);
    m->height = height;
    m->width = width;

    // Note this means all values of map are 0
    int *points = (int *)calloc(height * width, sizeof(int));
    assert(points);
    m->points = (int **)malloc(width * sizeof(int *));
    assert(m->points);

    for (int i = 0; i < width; i++)
    {
        /* Re-use sections of the memory we
            allocated. */
        m->points[i] = points + i * height;
    }

    return m;
}

struct point *newPoint(int x, int y)
{
    struct point *p = (struct point *)malloc(sizeof(struct point));
    assert(p);
    p->x = x;
    p->y = y;

    return p;
}

void freeMap(struct map *m)
{
    /* We only allocate one pointer with the rest
    of the pointers in m->points pointing inside the
    allocated memory. */
    free(m->points[0]);
    free(m->points);
    free(m);
}

void printMap(struct map *m)
{
    /* Print half of each row each time so we mirror the hexagonal layout. */
    int printRows = 2 * m->height;
    if (m->width < 2)
    {
        /* If the width is less than 2, simply a vertical column so no need to print
            last row as it will be empty. */
        printRows -= 1;
    }
    for (int i = 0; i < printRows; i++)
    {
        for (int j = i % 2; j < m->width; j += 2)
        {
            if (j == 1)
            {
                /* For odd row, add a spacer in to place the first value after the 0th hex
                    in the printout. */
                printf("       ");
            }
            /* Default to even. Select every second column. */
            int yPos = i / 2;
            if (j % 2 != 0)
            {
                /* If odd, numbering along height is reversed. */
                yPos = m->height - 1 - yPos;
            }
            int val = m->points[j][yPos];

            /* Print value appropriately. */
            if (val < 0)
            {
                printf("S%.3d", val % 1000);
            }
            else if (val == 0)
            {
                printf("  .  ");
            }
            else if (val == 100)
            {
                printf("  A  ");
            }
            else
            {
                printf("T+%.3d ", val % 1000);
            }
            printf("          ");
        }

        printf("\n");
    }
}

/* IMPLEMENT PART A HERE */
/* Function to sort by x coordinate */
int cmp_x(const void *a, const void *b)
{
    return (((struct point *)a)->x - ((struct point *)b)->x);
}
/* Function to sort by y coordinate */
int cmp_y(const void *a, const void *b)
{
    return (((struct point *)a)->y - ((struct point *)b)->y);
}
/* Note that for the implementation in this question, you will submit
   an array that contains all the adjacent points and then add an additional
   point at the end of the array with coordinates -1, -1 to signify the
   termination of the array (similar to '\0' terminating a string) */
struct point *getAdjacentPoints(struct map *m, struct point *p)
{
    struct point *ans = (struct point *)malloc(sizeof(struct point *) * 7); // multiply by 7 from 6 adjacent points + 1 end point
    assert(ans);
    ans->x = -1;
    ans->y = -1;
    int ans_num = 0;
    struct point temp;
    struct point temp2;
    /* Return if point is negative or point is outside of map dimensions */
    if (p->x >= m->width || p->x < 0 || p->y >= m->height || p->y < 0)
    {
        return ans;
    }
    /* Loop 3 times, each time returning 2 adjacent points */
    for (size_t i = 1; i < 4; i++)
    {
        temp.x = i + p->x - 2;
        temp2.x = temp.x;
        /* If even, assign the top and bottom value*/
        if (i % 2 == 0)
        {
            temp.y = p->y + 1;
            temp2.y = p->y - 1;
        }
        /* If odd, invert the height and assign top and bottom value */
        else
        {
            temp.y = (m->height - 1) - (p->y);
            temp2.y = (m->height - 1) - (p->y - 1);
        }
        /*  Testing if the adjacent point is positive and inside of map,
            adding it to array if condition passes */
        if (temp.x < m->width && temp.x >= 0 && temp.y < m->height && temp.y >= 0)
        {
            ans[ans_num] = temp;
            ans_num += 1;
        }
        if (temp2.x < m->width && temp2.x >= 0 && temp2.y < m->height && temp2.y >= 0)
        {
            ans[ans_num] = temp2;
            ans_num += 1;
        }
    }
    /* Sorting it in by the first coordinate, and then the second coordinate */
    qsort(ans, ans_num, sizeof(struct point), cmp_y);
    qsort(ans, ans_num, sizeof(struct point), cmp_x);
    /* Additional point at the end of the array with coordinates -1, -1 as flag values */
    ans[ans_num].x = -1;
    ans[ans_num].y = -1;

    return ans;
}

/* -------------------------------------------------------------------------- */

/* IMPLEMENT PART B HERE */
/* Function to check if a point has been visited*/
int is_visited(struct point *p, struct point *visited, int size)
{
    /* Loop through size of visited array */
    for (size_t i = 0; i < size; i++)
    {
        if (p->x == visited[i].x && p->y == visited[i].y)
        {
            return 1;
        }
    }
    return 0;
}

int mapValue(struct map *m)
{
    /* Initialising variables */
    struct point *visited = (struct point *)malloc(sizeof(struct point *) * (m->height * m->width));
    assert(visited);
    int visited_num = 0;
    int val = 0;
    int local_val = 0;
    int global_val = 0;
    int treasure_num = 0;

    struct pq *queue = createPQ();
    struct point start_p;
    struct point *temp;

    /* Iterating until all points has been visited */
    while (visited_num < (m->width * m->height))
    {
        val = 0;
        local_val = 1;
        treasure_num = 0;

        /* Iterating across entire map to find a starting point */
        for (size_t row = 0; row < m->width; row++)
        {
            for (size_t col = 0; col < m->height; col++)
            {
                start_p.x = row, start_p.y = col;
                val = m->points[start_p.x][start_p.y];
                if (!is_visited(&start_p, visited, visited_num) && val >= 0)
                {
                    if (val != 100 && val > 0)
                    {
                        local_val = val;
                        treasure_num += 1;
                    }
                    row = m->width; // break out of outer loop
                    break;          // break out of inner loop
                }
            }
        }

        /* Adding start point to queue and visited array to kickstart second loop */
        insert(queue, &start_p, 0);
        visited[visited_num] = start_p;
        visited_num += 1;

        while (!isEmpty(queue))
        {
            /* Getting point at the front of queue */
            temp = pull(queue);

            /* Getting adjacent points of the point pulled */
            struct point *adjPoints = getAdjacentPoints(m, temp);
            struct point *adjPoint = adjPoints;

            /*  Iterating until end point of (-1, -1),
                checks the value of an adjacent point if it has not been visited before,
                adds it to local_val if it is positive and not 100 */
            while (adjPoint->x != -1)
            {
                if (!is_visited(adjPoint, visited, visited_num))
                {
                    visited[visited_num] = *adjPoint;
                    visited_num += 1;
                    val = m->points[adjPoint->x][adjPoint->y];

                    /* If the point is a land, it is on the same island */
                    if (val >= 0)
                    {
                        if (val > 0 && val != 100)
                        {
                            local_val *= val;
                            treasure_num += 1;
                        }
                        insert(queue, adjPoint, 0);
                    }
                }
                adjPoint = adjPoint + 1;
            }
        }
        global_val += treasure_num * local_val;
    }
    free(visited);
    freePQ(queue);
    return global_val;
}

/* -------------------------------------------------------------------------- */

/* IMPLEMENT PART D HERE */
/* Function to get  time it takes to move to an adjacent point*/
int time(struct map *m, struct point *cur_p)
{
    int val = m->points[cur_p->x][cur_p->y];
    int time = 0;

    if (val < 0) // Sea
    {
        time = 2 + ceil((double)(val * val) / 1000);
    }
    else if (val == 100) // Airport
    {
        time = 5;
    }
    else // Land or Treasure
    {
        time = 5;
    }

    return time;
}
/* Function to convert a point coordinates to an index value*/
int point2index(struct map *m, struct point p)
{

    return ((m->height) * p.x) + p.y;
}
/* Function to get all the locations of airports in a map */
struct point *airport_list(struct map *m)
{
    int val, i = 0;
    struct point airport_p;
    struct point *airports = (struct point *)malloc(sizeof(struct point *) * (m->height * m->width));
    assert(airports);

    /* Iterating across entire map to find a starting point */
    for (size_t row = 0; row < m->width; row++)
    {
        for (size_t col = 0; col < m->height; col++)
        {
            airport_p.x = row, airport_p.y = col;
            val = m->points[airport_p.x][airport_p.y];
            if (val == 100)
            {
                airports[i] = airport_p;
                i += 1;
            }
        }
    }
    /* Additional point at the end of the array with coordinates -1, -1 as flag values */
    airports[i].x = -1;
    airports[i].y = -1;
    return airports;
}

/* Modified function of part A to include airports as adjacent points if the targeted point is an airport*/
struct point *getAdjacentPoints_with_airport(struct map *m, struct point *p, struct point *airports)
{
    struct point temp;
    struct point temp2;
    int ans_size = 8; // From 6 adjacent points + 2 end points
    int ans_num = 0;
    struct point *ans = (struct point *)malloc(sizeof(struct point *) * ans_size + (m->height * m->width));
    assert(ans);
    ans->x = -1;
    ans->y = -1;
    /* Same algorithm as Part A */
    if (p->x >= m->width || p->x < 0 || p->y >= m->height || p->y < 0)
    {
        return ans;
    }

    for (size_t i = 1; i < 4; i++)
    {
        temp.x = i + p->x - 2;
        temp2.x = temp.x;
        if (i % 2 == 0)
        {
            temp.y = p->y + 1;
            temp2.y = p->y - 1;
        }
        else
        {
            temp.y = (m->height - 1) - (p->y);
            temp2.y = (m->height - 1) - (p->y - 1);
        }

        if (temp.x < m->width && temp.x >= 0 && temp.y < m->height && temp.y >= 0)
        {

            ans[ans_num] = temp;
            ans_num += 1;
        }
        if (temp2.x < m->width && temp2.x >= 0 && temp2.y < m->height && temp2.y >= 0)
        {
            ans[ans_num] = temp2;
            ans_num += 1;
        }
    }
    /* Using (-2,-2) as a flag value for direct adjacent points */
    ans[ans_num].x = -2;
    ans[ans_num].y = -2;
    ans_num += 1;

    /* If the current point is an airport, check if there are other airports*/
    int val = m->points[p->x][p->y];
    if (val == 100)
    {
        /* Iterating through airport array to find airport that is not itself */
        struct point *airport = airports;
        while (airport->x != -1)
        {
            if (airport->x != p->x || airport->y != p->y)
            {
                ans[ans_num] = *airport;
                ans_num += 1;
            }
            airport = airport + 1;
        }
    }
    /* Using (-1,-1) as a flag value for airport adjacent points */
    ans[ans_num].x = -1;
    ans[ans_num].y = -1;

    return ans;
}

/* Function to find max value of 2 int */
int max(int x, int y)
{
    if (x > y)
        return x;
    return y;
}

/* Main part D function */
int minTime(struct map *m, struct point *start, struct point *end)
{
    struct point *airports = airport_list(m);
    int *p_cost = (int *)malloc(sizeof(int *) * (m->height * m->width));
    assert(p_cost);
    int *visit = (int *)malloc(sizeof(int *) * (m->height * m->width));
    assert(visit);

    /* Filling all elements of the array with a big value for cost array and 0 for visit array */
    for (size_t i = 0; i < (m->height * m->width); i++)
    {
        p_cost[i] = INF;
        visit[i] = 0;
    }

    /* Creating a priority queue and inserting the start point with the lowest priority */
    /*  As the priority queue struct given returns the item with the highest priority,
        but we want to find the lowest priority (lowest cost), all cost is taken as a negative value,
        meaning that the lower the cost, the higher the priority (as it is closer to 0) */
    struct pq *p_queue = createPQ();
    p_cost[point2index(m, *start)] = 0;
    insert(p_queue, start, 0);

    int index, new_cost, cur_cost;
    struct point *temp;

    /* Uses Dijkstra's Shortest Path Algorithm */
    while (!isEmpty(p_queue))
    {
        /* Peek is an added function to pq.c and pq.h to get the priority of the item that is about to be pulled */
        cur_cost = peek(p_queue);
        temp = pull(p_queue);
        index = point2index(m, *temp);
        visit[index] = 1; // Assigning the index position to the corresponding point to 1 to mark it has been visited

        struct point *adjPoints = getAdjacentPoints_with_airport(m, temp, airports);
        struct point *adjPoint = adjPoints;
        if (p_cost[index] <= cur_cost)
        {
            /* Iterating through all adjacent points (not connected airport)*/
            while (adjPoint->x != -2)
            {
                if (visit[point2index(m, *adjPoint)] != 1)
                {
                    /* Not adding but subtracting the time because we are working with negative values */
                    new_cost = p_cost[index] - time(m, temp);

                    /* Test if the new cost is faster than over the old one */
                    if (new_cost > p_cost[point2index(m, *adjPoint)])
                    {
                        p_cost[point2index(m, *adjPoint)] = new_cost;
                        insert(p_queue, adjPoint, new_cost);
                    }
                }
                adjPoint = adjPoint + 1;
            }
            /* After no more direct adjacent points */
            adjPoint = adjPoint + 1; // skipping flag point point (-2,-2)

            /* Iterating through all connected airport, if there is any */
            while (adjPoint->x != -1)
            {
                /* Performs the same operations as previous loop */
                if (visit[point2index(m, *adjPoint)] != 1)
                {
                    /* Since we know all adjacent points in this loop is a connected airport,
                     the max equation is used to find the time taken to get there */
                    new_cost = p_cost[index] - max(15, ((adjPoint->x - temp->x) * (adjPoint->x - temp->x) - 85));

                    if (new_cost > p_cost[point2index(m, *adjPoint)])
                    {
                        p_cost[point2index(m, *adjPoint)] = new_cost;
                        insert(p_queue, adjPoint, new_cost);
                    }
                }
                adjPoint = adjPoint + 1;
            }
        }
        /*  Return and terminate early if the index position = the end point's index
            as all possibility to the end from the start has been explored */
        if (index == point2index(m, *end))
        {
            free(visit);
            freePQ(p_queue);
            return -p_cost[point2index(m, *end)];
        }
    }
    free(visit);
    freePQ(p_queue);

    /* Returning a negative value of a negative value to make it positive again*/
    return -p_cost[point2index(m, *end)];
}

/* IMPLEMENT PART E HERE */
/* Function to calculate the cost of traveling between 2 land points in O(1) time*/
int point2point_cost(struct map *m, struct point start, struct point end)
{
    struct point a = start;
    struct point b = end;
    int cost = 0;

    /* If the x position is odd, flip the y coordinate */
    if (a.x % 2 != 0)
    {
        a.y = m->height - 1 - a.y;
    }
    if (b.x % 2 != 0)
    {
        b.y = m->height - 1 - b.y;
    }

    /* All land and sea point have a cost of 5 minute to travel to adjacent node */
    if (a.x == b.x) // same x position
    {
        cost = 5 * abs(a.y - b.y);
    }
    else if (a.y == b.y) // same y position
    {
        cost = 5 * abs(a.x - b.x);
    }

    /* Not sure how to explain this... but (b.x - a.x) is the fastest path to the y position of the end point,
     while (a.y - (((b.x - a.x) + 1) / 2)) is the distance left to cover after (b.x - a.x) */
    else if (a.x < b.x && a.y < b.y)
    {
        if ((a.x % 2) != 0)
        {
            if (a.y > b.y)
            {
                cost = 5 * ((b.x - a.x) + (a.y - (((b.x - a.x) + 1) / 2)));
            }
            else
            {
                cost = 5 * (b.x - a.x);
            }
        }
        else
        {
            if (a.y > b.y)
            {
                cost = 5 * (b.x - a.x) + (a.y - (((b.x - a.x)) / 2));
            }
            else
            {
                cost = 5 * (b.x - a.x);
            }
        }
    }
    else if (a.x < b.x && a.y > b.y)
    {
        if ((a.x % 2) != 0)
        {
            if (a.y < b.y)
            {
                cost = 5 * ((b.x - a.x) + (b.y - (((b.x - a.x) + 1) / 2)));
            }
            else
            {
                cost = 5 * (b.x - a.x);
            }
        }
        else
        {
            if (a.y < b.y)
            {
                cost = 5 * ((b.x - a.x) + (b.y - (((b.x - a.x)) / 2)));
            }
            else
            {
                cost = 5 * (b.x - a.x);
            }
        }
    }
    else if (a.x > b.x && a.y > b.y)
    {
        if ((a.x % 2) != 0)
        {
            if (a.y > b.y)
            {
                cost = 5 * ((a.x - b.x) + (b.y - (((b.x - a.x) + 1) / 2)));
            }
            else
            {
                cost = 5 * (a.x - b.x);
            }
        }
        else
        {
            if (a.y > b.y)
            {
                cost = 5 * ((a.x - b.x) + (b.y - (((b.x - a.x)) / 2)));
            }
            else
            {
                cost = 5 * (a.x - b.x);
            }
        }
    }
    else if (a.x > b.x && a.y < b.y)
    {
        if ((a.x % 2) != 0)
        {
            if (a.y > b.y)
            {
                cost = 5 * ((a.x - b.x) + (a.y - (((a.x - b.x) + 1) / 2)));
            }
            else
            {
                cost = 5 * (a.x - b.x);
            }
        }
        else
        {
            if (a.y > b.y)
            {
                cost = 5 * ((a.x - b.x) + (a.y - (((a.x - b.x)) / 2)));
            }
            else
            {
                cost = 5 * (a.x - b.x);
            }
        }
    }

    return cost;
}

int minTimeDry(struct map *m, struct point *start, struct point *end, struct point *airports, int numAirports)
{
    /* if no more than 2 airport, all points are treated as land, so distance can be calculated immediately*/
    if (numAirports < 2)
    {
        return point2point_cost(m, *start, *end);
    }
    struct point *airport_nearest_start;
    struct point *airport_nearest_end;

    int distance1 = -INF;
    int distance2 = -INF;

    /* Finding the airport nearest to the start and end point */
    for (size_t i = 0; i < numAirports; i++)
    {
        if (distance1 > (((start->x - airports[i].x) * (start->x - airports[i].x)) + ((start->y - airports[i].y) * (start->y - airports[i].y))))
        {
            airport_nearest_start = &airports[i];
            distance1 = ((start->x - airports[i].x) * (start->x - airports[i].x)) + ((start->y - airports[i].y) * (start->y - airports[i].y));
        }
        if (distance2 > ((end->x - airports[i].x) + (end->y - airports[i].y)))
        {
            airport_nearest_end = &airports[i];
            distance2 = ((end->x - airports[i].x) * (end->x - airports[i].x)) + ((end->y - airports[i].y) * (end->y - airports[i].y));
        }
    }
    /* Finding the time it takes to go from start point to its closest airport and end point to its closest airport*/
    int start2airport = point2point_cost(m, *start, *airport_nearest_start);
    int airport2end = point2point_cost(m, *airport_nearest_end, *end);

    int *p_cost = (int *)malloc(sizeof(int *) * (m->height * m->width));
    assert(p_cost);
    int *visit = (int *)malloc(sizeof(int *) * (m->height * m->width));
    assert(visit);

    /* Filling all elements of the array with a big value for cost array and 0 for visit array */
    for (size_t i = 0; i < (m->height * m->width); i++)
    {
        p_cost[i] = INF;
        visit[i] = 0;
    }

    struct pq *p_queue = createPQ();
    p_cost[point2index(m, *airport_nearest_start)] = 0;
    insert(p_queue, airport_nearest_start, 0);

    int index, new_cost, walk_cost, fly_cost;
    struct point *temp;

    /* Uses Dijkstra's Shortest Path Algorithm similar to part D to find shortest path between the
    airport nearest to start and airport nearest to end */
    while (!isEmpty(p_queue))
    {
        temp = pull(p_queue);
        index = point2index(m, *temp);
        visit[index] = 1;

        struct point *airport = airports;
        /* Iterating through all connected airport except for itself */

        for (size_t i = 0; i < numAirports; i++)
        {

            if (visit[point2index(m, *airport)] != 1 && temp != airport)
            {
                walk_cost = point2point_cost(m, *temp, *airport);
                fly_cost = max(15, ((airport->x - temp->x) * (airport->x - temp->x) - 85));

                /* Comparing the cost of flying and cost of walking*/
                if (walk_cost < fly_cost)
                {
                    new_cost = walk_cost;
                }
                else
                {
                    new_cost = fly_cost;
                }
                new_cost = p_cost[index] - new_cost;

                if (new_cost > p_cost[point2index(m, *airport)])
                {
                    p_cost[point2index(m, *airport)] = new_cost;
                    insert(p_queue, airport, new_cost);
                }
            }
            airport = airport + 1;
        }

        if (index == point2index(m, *airport_nearest_end))
        {
            free(visit);
            freePQ(p_queue);
            /* The cost of going to start airport and end airport is added before returning */
            return start2airport + (-p_cost[point2index(m, *airport_nearest_end)]) + airport2end;
        }
    }

    return start2airport + (-p_cost[point2index(m, *airport_nearest_end)]) + airport2end;
}
