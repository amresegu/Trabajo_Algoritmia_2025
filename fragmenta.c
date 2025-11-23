#include "fragmenta.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char **fragmenta(const char *e) {

    int tamano_de_tabla = 1;
    char *s1, *s2;
    char **salida = NULL;
    char *s;

    s1 = strdup(e);
    s2 = strdup(e);

    if (s1 == NULL || s2 == NULL) {
        fprintf(stderr, "ERROR: no se pudo duplicar la cadena en fragmenta\n");
        exit(EXIT_FAILURE);
    }

    s = strtok(s2, ",\r\n");
    while (s != NULL) {
        s = strtok(NULL, ",\r\n");
        tamano_de_tabla++;
    }

    salida = (char **)malloc(tamano_de_tabla * sizeof(char *));
    if (salida == NULL) {
        fprintf(stderr, "ERROR: no se pudo reservar memoria en fragmenta\n");
        free(s1);
        free(s2);
        exit(EXIT_FAILURE);
    }

    s = strtok(s1, ",\r\n");
    int i = 0;
    while (s != NULL) {
        salida[i] = strdup(s);
        if (salida[i] == NULL) {
            fprintf(stderr, "ERROR: no se pudo duplicar token en fragmenta\n");
            // Aquí podrías liberar lo ya reservado, pero para el trabajo puede valer así
            exit(EXIT_FAILURE);
        }
        s = strtok(NULL, ",\r\n");
        i++;
    }
    salida[tamano_de_tabla - 1] = NULL;   // centinela

    free(s1);
    free(s2);

    return salida;
}

void borrarg(char **arg)
{
    int i = 0;
    while (arg[i] != NULL) {
        free(arg[i]);
        i++;
    }
    free(arg);
}
