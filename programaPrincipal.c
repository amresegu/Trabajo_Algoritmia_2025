#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <float.h>
#include <ctype.h>
#include <stdbool.h>

#include "fragmenta.h"
#include "listaDatos.h"
#include "maxMonticulo.h"

#define MAX_CLASES 10   // nº máximo de clases distintas a considerar

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

/* ===================== FUNCIONES AUXILIARES ===================== */

static void a_minusculas(char *s)
{
    for (int i = 0; s[i] != '\0'; i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

// Limpia el buffer de entrada tras un scanf que falla o deja basura
static void limpiarBufferEntrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // descartar
    }
}

// Lee un float en un rango [min, max], repitiendo hasta que sea válido
static float leerFloatEnRango(const char *mensaje, float min, float max)
{
    float valor;
    int ok;

    do {
        printf("%s", mensaje);
        ok = scanf("%f", &valor);
        if (ok != 1) {
            printf("ERROR: Debe introducir un número.\n");
            limpiarBufferEntrada();
            continue;
        }
        if (valor < min || valor > max) {
            printf("ERROR: Valor fuera de rango [%.2f, %.2f].\n", min, max);
        }
    } while (ok != 1 || valor < min || valor > max);

    limpiarBufferEntrada(); // limpiar posible resto en la línea
    return valor;
}

// Lee un Yes/No (adaptado al dataset: "yes" / "no") y lo guarda en minúsculas
static void leerYesNo(const char *mensaje, char *dest)
{
    int valido;
    do {
        printf("%s", mensaje);
        if (scanf("%19s", dest) != 1) {
            printf("ERROR: Entrada no válida.\n");
            limpiarBufferEntrada();
            valido = 0;
            continue;
        }
        limpiarBufferEntrada();
        a_minusculas(dest);

        if (strcmp(dest, "yes") == 0 || strcmp(dest, "no") == 0) {
            valido = 1;
        } else {
            printf("ERROR: Debe escribir Yes o No.\n");
            valido = 0;
        }
    } while (!valido);
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

/* ===================== K = 1 CON TODOS ===================== */

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

/* ===================== EJEMPLO NUEVO ===================== */

void leerEjemploNuevo(Datos *d)
{
    printf("\n--- Introducción de un ejemplo nuevo ---\n");

    d->tiempo_a_solas =
        leerFloatEnRango("Tiempo a solas (0-11): ", 0.0f, 11.0f);

    leerYesNo("Miedo escénico (Yes/No): ", d->miedoEscenico);

    d->frec_asistencia_eventos =
        leerFloatEnRango("Asistencia a eventos (0-10): ", 0.0f, 10.0f);

    d->frec_salidas =
        leerFloatEnRango("Salir fuera (0-7): ", 0.0f, 7.0f);

    leerYesNo("Agotado tras socializar (Yes/No): ", d->agotamiento);

    d->n_amigos_cercanos =
        leerFloatEnRango("Número de amigos cercanos (0-15): ", 0.0f, 15.0f);

    d->frec_publicacion_redes =
        leerFloatEnRango("Frecuencia publicación redes (0-10): ", 0.0f, 10.0f);

    // En un ejemplo nuevo no conocemos la clase real
    d->resultado[0] = '\0';
}

/* ===================== K-NN GENÉRICO (CON MONTÍCULO) ===================== */

/* Decide la clase mayoritaria entre los K vecinos almacenados en el montículo */
static void claseMayoritariaDesdeMonticulo(tipoMaxMonticulo *mon, char *resultadoPredicho)
{
    char clasesDistintas[MAX_CLASES][TAM_RESULTADO];
    int conteo[MAX_CLASES];
    float minDist[MAX_CLASES];
    int clases = 0;

    if (mon->pos == 0) {
        resultadoPredicho[0] = '\0';
        return;
    }

    for (int i = 0; i < mon->pos; ++i) {
        if (mon->array[i].resultado[0] == '\0') {
            continue; // vecinos "vacíos"
        }

        int idx = -1;
        for (int j = 0; j < clases; ++j) {
            if (strcmp(clasesDistintas[j], mon->array[i].resultado) == 0) {
                idx = j;
                break;
            }
        }

        if (idx == -1) {
            if (clases >= MAX_CLASES) {
                continue;
            }
            idx = clases++;
            strcpy(clasesDistintas[idx], mon->array[i].resultado);
            conteo[idx] = 0;
            minDist[idx] = FLT_MAX;
        }

        conteo[idx]++;
        if (mon->array[i].distancia < minDist[idx]) {
            minDist[idx] = mon->array[i].distancia;
        }
    }

    int mejor = 0;
    for (int j = 1; j < clases; ++j) {
        if (conteo[j] > conteo[mejor] ||
            (conteo[j] == conteo[mejor] && minDist[j] < minDist[mejor])) {
            mejor = j;
        }
    }

    strcpy(resultadoPredicho, clasesDistintas[mejor]);
}

/* Predice la clase de un ejemplo usando KNN sobre la lista, opcionalmente ignorando una celda concreta */
static void predecirClaseKNN(const Datos *ejemplo, tipoLista *lista, int k,
                             const celdaLista *aIgnorar, char *resultadoPredicho)
{
    tipoMaxMonticulo mon;
    tipoElementoMaxMonticulo tupla;
    celdaLista *rec;

    if (k <= 0) {
        fprintf(stderr, "ERROR: K debe ser mayor que 0\n");
        exit(EXIT_FAILURE);
    }

    nuevoMaxMonticulo(&mon, k);

    tupla.distancia = FLT_MAX;
    tupla.resultado[0] = '\0';

    // Inicializamos el montículo con K tuplas "vacías"
    for (int i = 0; i < k; ++i) {
        insertarMaxMonticulo(&mon, tupla);
    }

    rec = lista->ini;
    while (rec != NULL) {
        if (rec != aIgnorar) {
            float distanciaActual = calcularDistancia(*ejemplo, rec);

            if (distanciaActual < devolverRaiz(mon).distancia) {
                eliminarElemento(&mon);
                tupla.distancia = distanciaActual;
                strcpy(tupla.resultado, rec->elem.resultado);
                insertarMaxMonticulo(&mon, tupla);
            }
        }
        rec = rec->sig;
    }

    claseMayoritariaDesdeMonticulo(&mon, resultadoPredicho);
    liberarMaxMonticulo(&mon);
}

void clasificarEjemploNuevoK(const Datos *nuevo, tipoLista *lista, int k)
{
    char resultadoPredicho[TAM_RESULTADO];

    predecirClaseKNN(nuevo, lista, k, NULL, resultadoPredicho);

    printf("\n--- Clasificación de ejemplo nuevo (K=%d) ---\n", k);
    printf("Clase predicha: %s\n", resultadoPredicho);
    printf("-------------------------------------------------\n");
}

/* Compara KNN sobre todo el conjunto (leave-one-out) */
float compararKNN (tipoLista* lista, int k, bool escribir)
{
    celdaLista* aComparar = lista->ini;
    float total = 0.0f;
    float acertados = 0.0f;

    if (lista->ini == NULL) {
        printf("ERROR: lista vacía, no se puede aplicar KNN\n");
        return 0.0f;
    }

    char resultadoPredicho[TAM_RESULTADO];

    while (aComparar != NULL)
    {
        predecirClaseKNN(&aComparar->elem, lista, k, aComparar, resultadoPredicho);

        if (escribir) {
            printf("Ejemplo real: %s, predicho: %s\n",
                   aComparar->elem.resultado, resultadoPredicho);
        }

        if (strcmp(aComparar->elem.resultado, resultadoPredicho) == 0) {
            acertados += 1.0f;
        }
        total += 1.0f;

        aComparar = aComparar->sig;
    }

    float porcentaje = (acertados / total) * 100.0f;

    if (escribir) {
        printf("\nTOTAL DATOS: %.0f, Aciertos: %.0f, Fallos: %.0f\n",
               total, acertados, total - acertados);
        printf("%.2f%% de aciertos\n", porcentaje);
    }

    return porcentaje;
}

/* ===================== WILSON (ENN) ===================== */

tipoLista wilson (tipoLista* lista, int mejork)
{
    tipoLista listaAdevolver;
    nuevaLista(&listaAdevolver);

    celdaLista *actual = lista->ini;
    char resultadoPredicho[TAM_RESULTADO];

    while (actual != NULL) {
        predecirClaseKNN(&actual->elem, lista, mejork, actual, resultadoPredicho);

        // Sólo mantenemos los ejemplos bien clasificados por KNN
        if (strcmp(actual->elem.resultado, resultadoPredicho) == 0) {
            insertar(&listaAdevolver, actual->elem);
        }

        actual = actual->sig;
    }

    // liberamos la lista original
    eliminarLista(lista);

    return listaAdevolver;
}

/* ===================== UTILIDADES LISTA (para estudio K vs N) ===================== */

int longitudLista(tipoLista *lista)
{
    int n = 0;
    celdaLista *it = lista->ini;
    while (it != NULL) {
        n++;
        it = it->sig;
    }
    return n;
}

// Crea una copia de los primeros N elementos de la lista original
tipoLista copiarPrimerosN(tipoLista *lista, int N)
{
    tipoLista nueva;
    nuevaLista(&nueva);

    celdaLista *it = lista->ini;
    int cont = 0;

    while (it != NULL && cont < N) {
        insertar(&nueva, it->elem);
        it = it->sig;
        cont++;
    }

    return nueva;
}

/* ===================== PROGRAMA PRINCIPAL ===================== */

int main (void)
{
    FILE* f;
    char buffer1[1000];
    char **fg;

    Datos datos;
    tipoLista lista;
    RangosAtributos rangos;

    int input, numMon;
    float porcentajeKNN;

    nuevaLista(&lista);
    inicializarRangos(&rangos);

    f = fopen("personality_datasert.csv", "r");
    if (f == NULL) {
        perror("Error al abrir el fichero");
        return 1;
    }

    // saltar cabecera
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

    /* Normalizar datos en la lista */
    celdaLista *it = lista.ini;
    while (it != NULL)
    {
        normalizarDatos (&it->elem, &rangos);
        it = it->sig;
    }

    /* Nuevo ejemplo */
    Datos ejemploNuevo;
    leerEjemploNuevo(&ejemploNuevo);
    normalizarDatos(&ejemploNuevo, &rangos);

    int kNuevo;
    do {
        printf("Elige K para clasificar el ejemplo nuevo (entero >=1): ");
        if (scanf("%d", &kNuevo) != 1) {
            printf("ERROR: Debe introducir un entero.\n");
            limpiarBufferEntrada();
            kNuevo = 0;
            continue;
        }
        limpiarBufferEntrada();
        if (kNuevo < 1) {
            printf("ERROR: K debe ser al menos 1.\n");
        }
    } while (kNuevo < 1);

    clasificarEjemploNuevoK(&ejemploNuevo, &lista, kNuevo);

    /* Menú de opciones */
    int salir = 0;

    while (!salir)
    {
        printf("\n¿Qué quieres hacer?\n");
        printf(" 0 - Salir\n");
        printf(" 1 - K = 1 con todos\n");
        printf(" 2 - KNN con K elegido\n");
        printf(" 3 - Wilson con la mejor K\n");
        printf(" 4 - Wilson con K cualquiera\n");
        printf(" 5 - Estudio precisión vs K y nº de ejemplos\n");
        printf("Opción: ");
        if (scanf("%d", &input) != 1) {
            printf("ERROR: Debe introducir un entero.\n");
            limpiarBufferEntrada();
            continue;
        }
        limpiarBufferEntrada();

        if (input == 0)
        {
            salir = 1;
        }
        else if (input == 1)
        {
            printf("\nClasificación de K=1 con todos los datos\n");
            compararK1todos (&lista);
        }
        else if (input == 2)
        {
            printf("¿Con cuántos vecinos quieres comparar (K)?: ");
            if (scanf("%d", &numMon) != 1) {
                printf("ERROR: Debe introducir un entero.\n");
                limpiarBufferEntrada();
                continue;
            }
            limpiarBufferEntrada();
            if (numMon < 1) {
                printf("ERROR: K debe ser al menos 1.\n");
                continue;
            }
            porcentajeKNN = compararKNN (&lista, numMon, true);
        }
        else if(input == 3)
        {
            printf("\nClasificación Wilson con la mejor K\n");

            float mejorPorcentaje;
            int mejorK;

            porcentajeKNN = compararKNN (&lista, 1, false);
            mejorPorcentaje = porcentajeKNN;
            mejorK = 1;

            // Ajusta el límite superior de K si quieres
            for(int i = 2; i < 98; i++)
            {
                porcentajeKNN = compararKNN (&lista, i, false);

                if (mejorPorcentaje < porcentajeKNN)
                {
                    mejorPorcentaje = porcentajeKNN;
                    mejorK = i;
                }
            }
            printf("Porcentaje: %.2f%%  Mejor K vecinos: %i\n",
                   mejorPorcentaje, mejorK);
            sleep(2);

            printf("\nAplicando clasificación Wilson...\n");
            lista = wilson (&lista, mejorK);
            imprimirLista (lista);
        }
        else if(input == 4)
        {
            printf("¿Con cuántos vecinos quieres comparar (K)?: ");
            if (scanf("%d", &numMon) != 1) {
                printf("ERROR: Debe introducir un entero.\n");
                limpiarBufferEntrada();
                continue;
            }
            limpiarBufferEntrada();
            if (numMon < 1) {
                printf("ERROR: K debe ser al menos 1.\n");
                continue;
            }

            lista = wilson (&lista, numMon);
            imprimirLista (lista);
        }
        else if (input == 5)
        {
            int total = longitudLista(&lista);
            int pasoEjemplos;

            printf("La lista tiene %d ejemplos.\n", total);
            printf("¿Cada cuántos ejemplos quieres muestrear? (p.ej. 100): ");
            if (scanf("%d", &pasoEjemplos) != 1) {
                printf("ERROR: Debe introducir un entero.\n");
                limpiarBufferEntrada();
                continue;
            }
            limpiarBufferEntrada();

            if (pasoEjemplos <= 0) pasoEjemplos = total;

            printf("\nN_ejemplos, K, Porcentaje_acierto\n");

            for (int n = pasoEjemplos; n <= total; n += pasoEjemplos)
            {
                tipoLista subLista = copiarPrimerosN(&lista, n);

                int Ks[] = {1, 3, 5, 7, 9};
                int numKs = (int)(sizeof(Ks)/sizeof(Ks[0]));

                for (int i = 0; i < numKs; ++i) {
                    int k = Ks[i];
                    if (k >= n) continue; // no tiene sentido K>=n

                    float porc = compararKNN(&subLista, k, false);
                    printf("%d, %d, %.2f\n", n, k, porc);
                }

                eliminarLista(&subLista);
            }
        }
        else
        {
            printf("Opción no válida.\n");
        }
    }

    /* Eliminar lista y salir */
    eliminarLista(&lista);
    return 0;
}
