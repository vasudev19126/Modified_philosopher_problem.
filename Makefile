compile1:
	gcc Blocking_2019126.c  -lpthread

run1: compile1
	./a.out

compile2:
	gcc Unblocking_2019126.c  -lpthread

run2: compile2
	./a.out