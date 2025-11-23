#ifndef LISTADATOS_H
#define LISTADATOS_H

#include <stdbool.h>

typedef struct Datos
{
    float tiempo_a_solas;
    char miedoEscenico[20];       
    float frec_asistencia_eventos;
    float frec_salidas;
    char agotamiento[20];
    float n_amigos_cercanos;
    float frec_publicacion_redes;
    char resultado[30];
} Datos;

typedef Datos tipoElementoLista;

typedef struct celdaL {
    tipoElementoLista elem;
    struct celdaL *sig;
} celdaLista;

typedef struct tipoL {
    celdaLista *ini;
    celdaLista *fin;
} tipoLista;

void nuevaLista(tipoLista *);

void insertar(tipoLista *, tipoElementoLista);

tipoElementoLista consultarPrimero(tipoLista);

tipoElementoLista consultarUltimo(tipoLista);

void eliminarLista(tipoLista *);

bool esNulaLista(tipoLista);

void imprimirLista(tipoLista lista);

#endif
