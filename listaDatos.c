#include "listaDatos.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void nuevaLista(tipoLista *l)
{
    l->ini = NULL;
    l->fin = NULL;
}

void insertar(tipoLista *l, tipoElementoLista e)
{
    celdaLista *nuevo = (celdaLista *)malloc(sizeof(celdaLista));
    if (nuevo == NULL) {
        fprintf(stderr, "ERROR: no se pudo reservar memoria para nueva celda\n");
        exit(EXIT_FAILURE);
    }

    nuevo->elem = e;
    nuevo->sig = NULL;

    if (esNulaLista(*l)) {
        l->ini = nuevo;
        l->fin = nuevo;
    } else {
        l->fin->sig = nuevo;
        l->fin = nuevo;
    }
}

tipoElementoLista consultarPrimero(tipoLista l)
{
    if (l.ini == NULL) {
        fprintf(stderr, "ERROR: consultarPrimero sobre lista vacia\n");
        exit(EXIT_FAILURE);
    }
    return l.ini->elem;
}

tipoElementoLista consultarUltimo(tipoLista l)
{
    if (l.fin == NULL) {
        fprintf(stderr, "ERROR: consultarUltimo sobre lista vacia\n");
        exit(EXIT_FAILURE);
    }
    return l.fin->elem;
}

bool esNulaLista(tipoLista l)
{
    return l.ini == NULL;
}

void eliminarLista(tipoLista *lista)
{
    celdaLista *actual = lista->ini;
    while (actual != NULL) {
        celdaLista *sig = actual->sig;
        free(actual);
        actual = sig;
    }
    lista->ini = NULL;
    lista->fin = NULL;
}

static void imprimir(Datos *datos)
{
    printf("%0.2f %s %0.2f %0.2f %s %0.2f %0.2f %s\n",
           datos->tiempo_a_solas,
           datos->miedoEscenico,
           datos->frec_asistencia_eventos,
           datos->frec_salidas,
           datos->agotamiento,
           datos->n_amigos_cercanos,
           datos->frec_publicacion_redes,
           datos->resultado);
}

void imprimirLista(tipoLista lista)
{
    celdaLista *auxRecorrido;
    int contador = 0;

    if (!esNulaLista(lista)) {
        auxRecorrido = lista.ini;
        while (auxRecorrido != NULL) {
            imprimir(&(auxRecorrido->elem));
            auxRecorrido = auxRecorrido->sig;
            contador++;
        }
        printf("Numero de filas: %d\n", contador);
    } else {
        printf("Lista vacia\n");
    }
}
