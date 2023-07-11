# Hexagonal-Path-Finder
Finding treasures in a hexagonal map and the minimal distance between one point to another (Main code implementation on map.c)

The tresure map is made out of hexagonal tiles:

<img width="360" alt="hexagon" src="https://github.com/ybotf/Hexagonal-Path-Finder/assets/56858161/b9155344-54b3-4062-9a96-44f5fd25f84b">

The points in a 7 × 4 array correspond to the points shown above. The same behaviour of odd and even columns in a different order generalises to an m × n
m × n map.

## Part A - Getting Adjacent Points
Create function *getAdjacentPoints() which returns all the adjacent points around an inputted point, sorted by first then second coordinate. If the inputted point is not on the map, the point is considered to have no adjacent points.

## Part B - Finding Treasures
Create function mapValue() that returns the total value of all the treasure found in a map. Each tile is assigned a value:
- If the value of the point is negative, it represents the depth of the ocean floor in metres. 
- If the value of the point is 0, it represents land without treasure. 
- If the value of the point is 100, it represents an airport. 
- If the value of the point is positive but not 100, it represents the value of that specific treasure.

The value of an island is given by multiple of the value of all treasures on that island (E.g. f(x) = x1 * x2 * x3, therefore the value of the map is the sum of the value of all islands.

Used BFS to explore all tiles.

## Part D - Finding Optimal Path
Create function minTime() which finds the minimum amount of time from a start point to the end point. The amount of time it takes to move to an adjacent point is determined by the current point you are on:
- It takes 5 minutes to move from a land point to an adjacent point.
- It takes 2 + ceil(depth^2 / 1000) time to move from a point in sea.
- It takes max(15, (x1 -x2)^2 - 85) minutes to fly from one airport to any other airport. Where x1 and x2 are the  x coordinates of the airports.
- It takes 5 minutes to move from an airport or treasure to an adjacent point (treat like land).

Used Dijkstra's Algorithm to find the shortest path

## Part E - Finding Optimal Path With Known Airports
Part E is an extention of part D but execpt the locations of all airports are known beforehand. This allows Part D's algorithm to find the minimum distance faster in average case.

(Altered description written by William Sutherland, 2023)
