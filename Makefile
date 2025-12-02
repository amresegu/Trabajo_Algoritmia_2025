all: programaPrincipal
	./programaPrincipal

programaPrincipal: programaPrincipal.c fragmenta.o listaDatos.o maxMonticulo.o
	gcc -Wall -g programaPrincipal.c fragmenta.o listaDatos.o maxMonticulo.o -lm -o programaPrincipal

fragmenta.o: fragmenta.c fragmenta.h
	gcc -Wall -g -c fragmenta.c

listaDatos.o: listaDatos.c listaDatos.h
	gcc -Wall -g -c listaDatos.c

maxMonticulo.o: maxMonticulo.c maxMonticulo.h listaDatos.h
	gcc -Wall -g -c maxMonticulo.c

# Ejecutar y guardar la salida en salida.txt
log: programaPrincipal
	./programaPrincipal | tee salida.txt

clean:
	rm -f fragmenta.o listaDatos.o maxMonticulo.o programaPrincipal salida.txt
