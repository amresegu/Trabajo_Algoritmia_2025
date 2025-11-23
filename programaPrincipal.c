#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>

#include "fragmenta.h"
#include "listaDatos.h"
#include "maxMonticulo.h"  

typedef struct {
    float min;
    float max;
} Rango;

typedef struct {
    Rango tiempo_a_solas;
    Rango frec_asistencia_eventos;
    Rango frec_salidas;
    Rango n_amigos_cercanos;
    Rango frec_publicacion_redes;
} RangosAtributos;

static void a_minusculas(char *s)
{
    for (int i = 0; s[i] != '\0'; i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

static void inicializarRangos(RangosAtributos *r)
{
    r->tiempo_a_solas.min           = FLT_MAX;  r->tiempo_a_solas.max           = -FLT_MAX;
    r->frec_asistencia_eventos.min  = FLT_MAX;  r->frec_asistencia_eventos.max  = -FLT_MAX;
    r->frec_salidas.min             = FLT_MAX;  r->frec_salidas.max             = -FLT_MAX;
    r->n_amigos_cercanos.min        = FLT_MAX;  r->n_amigos_cercanos.max        = -FLT_MAX;
    r->frec_publicacion_redes.min   = FLT_MAX;  r->frec_publicacion_redes.max   = -FLT_MAX;
}

static void actualizarRango(Rango *r, float v)
{
    if (v < r->min) r->min = v;
    if (v > r->max) r->max = v;
}

static void actualizarRangos(RangosAtributos *r, const Datos *d)
{
    actualizarRango(&r->tiempo_a_solas,          d->tiempo_a_solas);
    actualizarRango(&r->frec_asistencia_eventos, d->frec_asistencia_eventos);
    actualizarRango(&r->frec_salidas,            d->frec_salidas);
    actualizarRango(&r->n_amigos_cercanos,       d->n_amigos_cercanos);
    actualizarRango(&r->frec_publicacion_redes,  d->frec_publicacion_redes);
}

static float normalizarValor(float v, Rango r)
{
    if (r.max == r.min) {
        return 0.0f;
    }
    return (v - r.min) / (r.max - r.min);
}

static void normalizarDatos(Datos *d, const RangosAtributos *r)
{
    d->tiempo_a_solas          = normalizarValor(d->tiempo_a_solas,          r->tiempo_a_solas);
    d->frec_asistencia_eventos = normalizarValor(d->frec_asistencia_eventos, r->frec_asistencia_eventos);
    d->frec_salidas            = normalizarValor(d->frec_salidas,            r->frec_salidas);
    d->n_amigos_cercanos       = normalizarValor(d->n_amigos_cercanos,       r->n_amigos_cercanos);
    d->frec_publicacion_redes  = normalizarValor(d->frec_publicacion_redes,  r->frec_publicacion_redes);
}

void meterDatosStruct(Datos* datos, char** fg)
{
    datos->tiempo_a_solas          = atof(fg[0]);
    strcpy(datos->miedoEscenico,   fg[1]);
    datos->frec_asistencia_eventos = atof(fg[2]);
    datos->frec_salidas            = atof(fg[3]);
    strcpy(datos->agotamiento,     fg[4]);
    datos->n_amigos_cercanos       = atof(fg[5]);
    datos->frec_publicacion_redes  = atof(fg[6]);
    strcpy(datos->resultado,       fg[7]);
    
    a_minusculas(datos->miedoEscenico);
    a_minusculas(datos->agotamiento);
    a_minusculas(datos->resultado);
}

float calcularDistancia(Datos dato, celdaLista* celda)
{
    float total = 0.0f;

    total += (dato.tiempo_a_solas - celda->elem.tiempo_a_solas)
           * (dato.tiempo_a_solas - celda->elem.tiempo_a_solas);

    if (strcmp(dato.miedoEscenico, celda->elem.miedoEscenico) != 0) {
        total += 1.0f;
    }

    total += (dato.frec_asistencia_eventos - celda->elem.frec_asistencia_eventos)
           * (dato.frec_asistencia_eventos - celda->elem.frec_asistencia_eventos);

    total += (dato.frec_salidas - celda->elem.frec_salidas)
           * (dato.frec_salidas - celda->elem.frec_salidas);

    if (strcmp(dato.agotamiento, celda->elem.agotamiento) != 0) {
        total += 1.0f;
    }

    total += (dato.n_amigos_cercanos - celda->elem.n_amigos_cercanos)
           * (dato.n_amigos_cercanos - celda->elem.n_amigos_cercanos);

    total += (dato.frec_publicacion_redes - celda->elem.frec_publicacion_redes)
           * (dato.frec_publicacion_redes - celda->elem.frec_publicacion_redes);

    return sqrtf(total);
}

void compararK1todos(tipoLista* lista)
{
    celdaLista* aComparar = lista->ini;
    celdaLista* recorrido;

    float distanciaMinima;
    float distanciaActual;

    char resultadoReal[TAM_RESULTADO];
    char resultadoPredicho[TAM_RESULTADO];

    float total = 0.0f;
    float acertados = 0.0f;

    while (aComparar != NULL)
    {
        distanciaMinima = FLT_MAX;

        strcpy(resultadoReal, aComparar->elem.resultado);

        recorrido = lista->ini;

        while (recorrido != NULL)
        {
            if (recorrido != aComparar)
            {
                distanciaActual = calcularDistancia(aComparar->elem, recorrido);

                if (distanciaActual < distanciaMinima)
                {
                    distanciaMinima = distanciaActual;
                    strcpy(resultadoPredicho, recorrido->elem.resultado);
                }
            }
            recorrido = recorrido->sig;
        }

        printf("Distancia mínima: %.4f, Valor real: %s, Valor predicho: %s\n",
               distanciaMinima, resultadoReal, resultadoPredicho);

        if (strcmp(resultadoReal, resultadoPredicho) == 0)
        {
            acertados += 1.0f;
        }
        total += 1.0f;

        aComparar = aComparar->sig;
    }

    float fallos = total - acertados;
    float porcentaje = (acertados / total) * 100.0f;

    printf("\n\nTotal datos: %.0f, Aciertos: %.0f, Fallos: %.0f", total, acertados, fallos);
    printf("\n%.2f%% de acierto\n", porcentaje);
}

void leerEjemploNuevo(Datos *d)
{
    printf("\n--- Introducción de un ejemplo nuevo ---\n");
    printf("Tiempo a solas (0-11): ");
    scanf("%f", &d->tiempo_a_solas);

    printf("Miedo escénico (Yes/No): ");
    scanf("%19s", d->miedoEscenico);

    printf("Asistencia a eventos (0-10): ");
    scanf("%f", &d->frec_asistencia_eventos);

    printf(""Salir fuera (0-7): ");
    scanf("%f", &d->frec_salidas);

    printf("Agotado tras socializar (Yes/No): ");
    scanf("%19s", d->agotamiento);

    printf("Número de amigos cercanos (0-15): ");
    scanf("%f", &d->n_amigos_cercanos);

    printf("Frecuencia publicación redes (0-10): ");
    scanf("%f", &d->frec_publicacion_redes);

    d->resultado[0] = '\0';

    a_minusculas(d->miedoEscenico);
    a_minusculas(d->agotamiento);
}

void clasificarEjemploNuevoK1(const Datos *nuevo, tipoLista *lista)
{
    celdaLista *rec = lista->ini;
    float distanciaMinima = FLT_MAX;
    float distanciaActual;
    char resultadoPredicho[TAM_RESULTADO];

    if (rec == NULL) {
        printf("ERROR: lista vacía, no se puede clasificar.\n");
        return;
    }

    while (rec != NULL) {
        distanciaActual = calcularDistancia(*nuevo, rec);
        if (distanciaActual < distanciaMinima) {
            distanciaMinima = distanciaActual;
            strcpy(resultadoPredicho, rec->elem.resultado);
        }
        rec = rec->sig;
    }

    printf("\n--- Clasificación de ejemplo nuevo (K=1) ---\n");
    printf("Clase predicha: %s\n", resultadoPredicho);
    printf("Distancia mínima: %.4f\n", distanciaMinima);
}

int main (void)
{
    FILE* f;
    char buffer1[1000];
    char **fg;

    Datos datos;
    tipoLista lista;
    RangosAtributos rangos;

    nuevaLista(&lista);
    inicializarRangos(&rangos);

    f = fopen("personality_datasert.csv", "r");
    if (f == NULL) {
        perror("Error al abrir el fichero");
        return 1;
    }

    if (fgets(buffer1, sizeof(buffer1), f) == NULL) {
        fprintf(stderr, "ERROR: fichero vacío o con error de lectura\n");
        fclose(f);
        return 1;
    }
    
    while (fgets(buffer1, sizeof(buffer1), f) != NULL)
    {
        fg = fragmenta(buffer1);

        meterDatosStruct(&datos, fg);  
        actualizarRangos(&rangos, &datos);
        insertar(&lista, datos);

        borrarg(fg);
    }
    fclose(f);
    
    celdaLista *it = lista.ini;
    while (it != NULL) {
        normalizarDatos(&it->elem, &rangos);
        it = it->sig;
    }

    Datos ejemploNuevo;
    leerEjemploNuevo(&ejemploNuevo);
    normalizarDatos(&ejemploNuevo, &rangos);
    clasificarEjemploNuevoK1(&ejemploNuevo, &lista);

    printf ("\n\nClasificación de K1 con todos\n");
    compararK1todos(&lista);

    eliminarLista(&lista);

    return 0;
}


