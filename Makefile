all:
	gcc src/*.c -I include/ -o main -g -Wall

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

zip:
	zip -r trab2.zip * "*.zip"
	
run:
	./main
