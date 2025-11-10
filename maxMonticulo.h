#include <stdbool.h>

typedef struct tipoElementoMaxMonticulo{
	float  distancia;
	int alcohol;
}tipoElementoMaxMonticulo;

typedef struct tMM{
	tipoElementoMaxMonticulo* array;
	int pos;
	int numEl;
}tipoMaxMonticulo;

void nuevoMaxMonticulo(tipoMaxMonticulo*, int);

void insertarMaxMonticulo(tipoMaxMonticulo *, tipoElementoMaxMonticulo);

void eliminarElemento(tipoMaxMonticulo *);

tipoElementoMaxMonticulo devolverRaiz(tipoMaxMonticulo);

void mostrarAnchura(tipoMaxMonticulo);

bool esVacio(tipoMaxMonticulo);

bool estaLleno(tipoMaxMonticulo);
