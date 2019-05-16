all:
	gcc list.c utils.c options.c main.c -Wall -o main

main:
	gcc main.c -o main

utils:
	gcc utils.c -o utils

options:
	gcc options.c -o options

list:
	gcc list.c -o list

mem:
	valgrind --leak-check=full ./main

run:
	./main
