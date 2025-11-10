#include "listaDatos.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void nuevaLista(tipoLista * l)
{
    l -> ini = NULL;
    l -> fin = NULL;
}

void insertar(tipoLista * l, tipoElementoLista e)
{
    celdaLista* nuevo;

    if (esNulaLista(*l))
    {
        nuevo = (celdaLista*)malloc(sizeof(celdaLista));
        nuevo -> elem = e;
        l -> ini = nuevo;
        l -> fin = nuevo;
        nuevo -> sig = NULL;
    }
    else
    {
        nuevo = (celdaLista*)malloc(sizeof(celdaLista));
        nuevo -> elem = e;
        l -> fin -> sig = nuevo;
        l -> fin = nuevo;
        nuevo -> sig = NULL;
    }
}


tipoElementoLista consultarPrimero(tipoLista l)
{
    return l.ini -> elem;
}

tipoElementoLista consultarUltimo(tipoLista l)
{
    return l.fin -> elem;
}


bool esNulaLista(tipoLista l)
{
    return l.ini == NULL;
}

void eliminarLista(tipoLista *lista)
{
    celdaLista* auxRecorrido;
    celdaLista* auxRecorridoSig;
    if(!esNulaLista(*lista))
    {
        auxRecorrido = lista -> ini;
        auxRecorridoSig = auxRecorrido -> sig;
        while(auxRecorridoSig != NULL)
        {
            free(auxRecorrido);
            auxRecorrido = auxRecorridoSig;
            auxRecorridoSig = auxRecorridoSig -> sig;
        }
        free(auxRecorrido);
        lista = NULL;
    }
}

void imprimir(Datos* datos)
{
    printf("%0.2f %s %0.2f %0.2f %s %0.2f %0.2f %s\n", datos->tiempo_a_solas, datos->miedoEscenico, datos->frec_asistencia_eventos, datos->frec_salidas, datos->agotamiento, datos->n_amigos_cercanos, datos->frec_publicacion_redes, datos->resultado);
}

void imprimirLista(tipoLista lista)
{
    celdaLista* auxRecorrido;
    int contador;
    if(!esNulaLista(lista))
    {
        auxRecorrido = lista.ini;
        contador = 0;
        while(auxRecorrido != NULL)
        {
            imprimir(&(auxRecorrido -> elem));
            auxRecorrido = auxRecorrido -> sig;
            contador++;
        }
        printf("Numero de filas: %d\n",contador);
    }
}

