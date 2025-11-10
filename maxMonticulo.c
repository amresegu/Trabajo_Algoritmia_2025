#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "maxMonticulo.h"

void nuevoMaxMonticulo(tipoMaxMonticulo* t, int nume)
{
    t->array = (tipoElementoMaxMonticulo*)malloc(sizeof(tipoElementoMaxMonticulo) * nume);
    t->pos = 0;
    t->numEl = nume;
}

void insertarMaxMonticulo(tipoMaxMonticulo *t, tipoElementoMaxMonticulo e)
{
    if (estaLleno(*t))
    {
            printf("\nERROR: El monticulo esta lleno\n");
    }
    else
    {
        tipoElementoMaxMonticulo aux;
        int i;
        t->array[t->pos] = e;
        t->pos++;
        i = t->pos - 1;
        while (e.distancia > t->array[(i-1) / 2].distancia)
        {
            aux = t->array[i];
            t->array[i] = t->array[(i-1) / 2];
            t->array[(i-1) / 2] = aux;
            i = (i-1) / 2;
        }
    }
}

void eliminarElemento(tipoMaxMonticulo *t)
{
    if (!esVacio(*t))
    {
        t -> array[0] = t -> array[t -> pos - 1];
        t -> pos = t -> pos - 1;
        int i = 0;
        tipoElementoMaxMonticulo aux;
        while((t->array[i].distancia < t->array[2*i+2].distancia && 2 * i + 2 <= t -> pos)|| (t->array[i].distancia < t->array[2*i+1].distancia && 2 * i + 2 <= t -> pos))
        {
            if (t->array[2*i+2].distancia > t->array[2*i+1].distancia)
            {
                aux = t->array[2*i+2];
                t->array[2*i+2] = t->array[i];
                t->array[i] = aux;
                i = 2 * i + 2;
            }
            else
            {
                aux = t->array[2*i+1];
                t->array[2*i+1] = t->array[i];
                t->array[i] = aux;
                i = 2 * i + 1;
            }
        }
    }
    else
    {
        printf("\nERROR: No se puede eliminar en monticulo vacio\n");

    }
}

tipoElementoMaxMonticulo devolverRaiz(tipoMaxMonticulo t)
{
    if (!esVacio(t))
    {
        return t.array[0];
    }
    else
    {
        printf("\nERROR: No hay raiz en monticulo vacio\n");
        exit(-1);
    }
}

void mostrarAnchura(tipoMaxMonticulo t)
{
    for(int i = 0; i < t.pos; i++)
    {
        printf("%f ", t.array[i].distancia);
        printf("%d\n", t.array[i].alcohol);
    }
}

bool esVacio(tipoMaxMonticulo t)
{
    return t.pos == 0;
}

bool estaLleno(tipoMaxMonticulo t)
{
    return t.pos == t.numEl;
}
