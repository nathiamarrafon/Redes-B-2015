enlace : garbler.o enlace.o testeEnlace.o main.o 
	gcc main.o garbler.o enlace.o testeEnlace.o -o enlace -lcrypt -lpthread

garbler.o : garbler.c
	gcc -c garbler.c

enlace.o : enlace.c
	gcc -c enlace.c -lcrypt -lpthread

testeEnlace.o : testeEnlace.c
	gcc -c testeEnlace.c -lpthread

main.o : main.c
	gcc -c main.c -lpthread

clean :
	rm testeEnlace.o enlace.o garbler.o main.o