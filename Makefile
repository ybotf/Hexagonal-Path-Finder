problem2a: problem2a.o problem.o map.o stack.o pq.o
	gcc -Wall -o problem2a problem2a.o problem.o map.o stack.o pq.o -g

problem2b: problem2b.o problem.o map.o stack.o pq.o
	gcc -Wall -o problem2b problem2b.o problem.o map.o stack.o pq.o -g

problem2d: problem2d.o problem.o map.o stack.o pq.o
	gcc -Wall -o problem2d problem2d.o problem.o map.o stack.o pq.o -g

problem2e: problem2e.o problem.o map.o stack.o pq.o
	gcc -Wall -o problem2e problem2e.o problem.o map.o stack.o pq.o -g
	

problem2a.o: problem2a.c problem.h map.h stack.h pq.h
	gcc -c problem2a.c -Wall -g

problem2b.o: problem2b.c problem.h map.h stack.h pq.h
	gcc -c problem2b.c -Wall -g

problem2d.o: problem2d.c problem.h map.h stack.h pq.h
	gcc -c problem2d.c -Wall -g

problem2e.o: problem2e.c problem.h map.h stack.h pq.h
	gcc -c problem2e.c -Wall -g

problem.o: problem.c problem.h map.h stack.h pq.h
	gcc -c problem.c -Wall -g

map.o: map.c map.h stack.h pq.h
	gcc -c map.c -Wall -g

stack.o: stack.c stack.h
	gcc -c stack.c -Wall -g

pq.o: pq.c pq.h
	gcc -c pq.c -Wall -g
