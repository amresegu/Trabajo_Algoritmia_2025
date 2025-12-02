#ifndef MAXMONTICULO_H
#define MAXMONTICULO_H

#include <stdbool.h>
#include "listaDatos.h"   

typedef struct tipoElementoMaxMonticulo {
    float distancia;
    char resultado[TAM_RESULTADO];   
} tipoElementoMaxMonticulo;

typedef struct tMM {
    tipoElementoMaxMonticulo* array;  
    int pos;  
    int numEl;        
} tipoMaxMonticulo;

void nuevoMaxMonticulo(tipoMaxMonticulo* m, int capacidad);

void insertarMaxMonticulo(tipoMaxMonticulo* m, tipoElementoMaxMonticulo e);

void eliminarElemento(tipoMaxMonticulo* m);

tipoElementoMaxMonticulo devolverRaiz(tipoMaxMonticulo m);

void mostrarAnchura(tipoMaxMonticulo m);

bool esVacio(tipoMaxMonticulo m);

bool estaLleno(tipoMaxMonticulo m);

void liberarMaxMonticulo(tipoMaxMonticulo *m);

#endif




