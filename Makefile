all: programaPrincipal fragmenta lista maxMonticulo
	./programaPrincipal

programaPrincipal: fragmenta lista maxMonticulo
	gcc programaPrincipal.c fragmenta.o listaDatos.o maxMonticulo.o -o programaPrincipal -Wall -lm -g

fragmenta:
	gcc -c fragmenta.c -Wall -g

lista:
	gcc -c listaDatos.c -Wall -g

maxMonticulo:
	gcc -c maxMonticulo.c -Wall -g

clean:
	rm fragmenta.o programaPrincipal listaDatos.o maxMonticulo.o
