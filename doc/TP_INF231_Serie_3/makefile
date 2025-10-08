all: ppmviewer

ppmviewer: main.o ppm.o
	gcc -o ppmviewer main.o ppm.o

main.o: main.c ppm.h
	gcc -c main.c

ppm.o: ppm.c ppm.h
	gcc -c ppm.c

clean:
	rm -f *.o ppmviewer

