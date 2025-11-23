#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <math.h>

#include "fragmenta.h"

#include "listaDatos.h"
#include "maxMonticulo.h"

#define MAX 999999

typedef struct compFinal{
    int cant;
    float distMin;
}compFinal;


// Funcion normalizar datos

void normalizar (Datos* datos)
{
    datos -> tiempo_a_solas = (datos -> tiempo_a_solas - 0) / (11 - 0);
    datos -> frec_asistencia_eventos = (datos -> frec_asistencia_eventos - 0) / (10 - 0);
    datos -> frec_salidas = (datos -> frec_salidas - 0) / (7 - 0);
    datos -> n_amigos_cercanos = (datos -> n_amigos_cercanos - 0) / (15 - 0);
    datos -> frec_publicacion_redes = (datos -> frec_publicacion_redes - 0) / (10 - 0);
}

// Funcion meter Datos al Struct

void meterDatosStruct (Datos* datos, char** fg)
{
    datos -> tiempo_a_solas = atof(fg[0]);
    strcpy(datos -> miedoEscenico, fg[1]);
    datos -> frec_asistencia_eventos = atof(fg[2]);
    datos -> frec_salidas = atof(fg[3]);
    strcpy(datos -> agotamiento, fg[4]);
    datos -> n_amigos_cercanos = atof(fg[5]);
    datos -> frec_publicacion_redes = atof(fg[6]);
    strcpy(datos -> resultado, fg[7]);
}

// Funcion Calcular Distancia entre ejemplos

float calcularDistancia (Datos dato, celdaLista* celda)
{
    float total;
    total = 0;

    total += (dato.tiempo_a_solas - celda->elem.tiempo_a_solas) * (dato.tiempo_a_solas - celda->elem.tiempo_a_solas);

    if (strcmp(dato.miedoEscenico, celda->elem.miedoEscenico) != 0)
    {
        total += 1 * 1;
    }

    total += (dato.frec_asistencia_eventos - celda->elem.frec_asistencia_eventos) * (dato.frec_asistencia_eventos - celda->elem.frec_asistencia_eventos);

    total += (dato.frec_salidas - celda->elem.frec_salidas) * (dato.frec_salidas - celda->elem.frec_salidas);

    if (strcmp(dato.agotamiento, celda->elem.agotamiento) != 0)
    {
        total += 1 * 1;
    }

    total += (dato.n_amigos_cercanos - celda->elem.n_amigos_cercanos) * (dato.n_amigos_cercanos - celda->elem.n_amigos_cercanos);

    total += (dato.frec_publicacion_redes - celda->elem.frec_publicacion_redes) * (dato.frec_publicacion_redes - celda->elem.frec_publicacion_redes);

    return sqrtf(total);
}

// Funcion K1 con todos

void compararK1todos(tipoLista* lista)
{
    celdaLista* aComparar = lista->ini;
    celdaLista* recorrido;

    float distanciaMinima;
    float distanciaActual;

    char resultadoReal[15];
    char resultadoPredicho[15];

    float total = 0;
    float acertados = 0;

    while (aComparar != NULL)
    {
        distanciaMinima = MAX;

        // Guardamos la etiqueta real

        strcpy (resultadoReal, aComparar->elem.resultado);

        recorrido = lista -> ini;

        while (recorrido != NULL)
        {
            if (recorrido != aComparar)
            {
                distanciaActual = calcularDistancia (aComparar->elem, recorrido);

                if (distanciaActual < distanciaMinima)
                {
                    distanciaMinima = distanciaActual;
                    strcpy(resultadoPredicho, recorrido->elem.resultado);
                }
            }
            recorrido = recorrido->sig;
        }

        printf("Distancia mínima: %.4f, Valor real: %s, Valor Predecido: %s\n", distanciaMinima, resultadoReal, resultadoPredicho);

        if (strcmp(resultadoReal, resultadoPredicho) == 0)
        {
            acertados = acertados + 1;
        }
        total = total + 1;

        aComparar = aComparar->sig;
    }

    float fallos = total - acertados;
    float porcentaje = (acertados / total) * 100.0f;

    printf("\n\nTotal Datos: %.0f, Aciertos: %.0f, Fallos: %.0f", total, acertados, fallos);
    printf("\n%.2f%% de acierto\n", porcentaje);
}

int main (void)
{
    FILE* f;
    char buffer1[1000];
    char **fg;

    struct Datos datos;

    tipoLista lista;
    nuevaLista(&lista);

    f = fopen("personality_datasert.csv", "r");
    if (f == NULL) {
        perror("Error al abrir el fichero");
        return 1;
    }
    
    fgets(buffer1, 1000, f);

    while (fgets(buffer1, 1000, f) != NULL)
    {
        fg = fragmenta(buffer1);
    
        meterDatosStruct(&datos, fg);
        normalizar(&datos);
    
        insertar(&lista, datos);
    
        borrarg(fg);
    }
    
    printf ("Clasificación de K1 con todos\n");
    compararK1todos(&lista);

    fclose (f);
}


