#include <stdbool.h>

typedef struct Datos
{
    float tiempo_a_solas;
    char miedoEscenico [10];
    float frec_asistencia_eventos;
    float frec_salidas;
    char agotamiento [10];
    float n_amigos_cercanos;
    float frec_publicacion_redes;
    char resultado [20];
}Datos;

typedef Datos tipoElementoLista;

typedef struct celdaL{
	tipoElementoLista elem;
	struct celdaL *sig;
}celdaLista; 
typedef struct tipoL{
	celdaLista *ini;
	celdaLista *fin;
}tipoLista;

void nuevaLista(tipoLista *);

void insertar(tipoLista *, tipoElementoLista);

tipoElementoLista consultarPrimero(tipoLista);

tipoElementoLista consultarUltimo(tipoLista);

void eliminarLista(tipoLista *);

bool esNulaLista(tipoLista);

void imprimirLista(tipoLista lista);

