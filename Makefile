compile:
	gcc Blocking.c  -lpthread

run: compile
	./a.out