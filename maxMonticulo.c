#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "maxMonticulo.h"

void nuevoMaxMonticulo(tipoMaxMonticulo* m, int capacidad)
{
    m->array = (tipoElementoMaxMonticulo*)malloc(sizeof(tipoElementoMaxMonticulo) * capacidad);
    if (m->array == NULL) {
        fprintf(stderr, "Error al reservar memoria para el monticulo\n");
        exit(EXIT_FAILURE);
    }
    m->pos = 0;
    m->numEl = capacidad;
}

bool esVacio(tipoMaxMonticulo m)
{
    return m.pos == 0;
}

bool estaLleno(tipoMaxMonticulo m)
{
    return m.pos == m.numEl;
}

void insertarMaxMonticulo(tipoMaxMonticulo* m, tipoElementoMaxMonticulo e)
{
    if (estaLleno(*m)) {
        printf("\nERROR: El monticulo esta lleno\n");
        return;
    }

    int i = m->pos;
    m->array[i] = e;
    m->pos++;

    while (i > 0) {
        int padre = (i - 1) / 2;
        if (m->array[i].distancia <= m->array[padre].distancia)
            break;

        tipoElementoMaxMonticulo aux = m->array[i];
        m->array[i] = m->array[padre];
        m->array[padre] = aux;

        i = padre;
    }
}

void eliminarElemento(tipoMaxMonticulo* m)
{
    if (esVacio(*m)) {
        printf("\nERROR: No se puede eliminar en monticulo vacio\n");
        return;
    }

    m->array[0] = m->array[m->pos - 1];
    m->pos--;

    int i = 0;

    while (1) {
        int hijoIzq = 2 * i + 1;
        int hijoDer = 2 * i + 2;
        int mayor = i;

        if (hijoIzq < m->pos &&
            m->array[hijoIzq].distancia > m->array[mayor].distancia) {
            mayor = hijoIzq;
        }

        if (hijoDer < m->pos &&
            m->array[hijoDer].distancia > m->array[mayor].distancia) {
            mayor = hijoDer;
        }

        if (mayor == i) {
            break;
        }

        tipoElementoMaxMonticulo aux = m->array[i];
        m->array[i] = m->array[mayor];
        m->array[mayor] = aux;

        i = mayor;
    }
}

tipoElementoMaxMonticulo devolverRaiz(tipoMaxMonticulo m)
{
    if (!esVacio(m)) {
        return m.array[0];
    } else {
        printf("\nERROR: No hay raiz en monticulo vacio\n");
        exit(EXIT_FAILURE);
    }
}

void mostrarAnchura(tipoMaxMonticulo m)
{
    for (int i = 0; i < m.pos; i++) {
        printf("distancia = %f, resultado = %s\n",
               m.array[i].distancia,
               m.array[i].resultado);
    }
}
