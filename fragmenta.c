
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

    s = strtok(s2, ",\n");
    while (s != NULL) {
        s = strtok(NULL, ",\n");
        tamano_de_tabla++;
    }

    salida = (char **)malloc((tamano_de_tabla) * sizeof(char *));

    s = strtok(s1, ",\n");
    int i = 0;
    while (s != NULL) {
        salida[i] = strdup(s);
        s = strtok(NULL, ",\n");
        i = i + 1;
    }
    salida[tamano_de_tabla - 1] = NULL;

    free(s1);
    free(s2);

    return salida;
}

void borrarg(char ** arg)
{
    int i = 0;
    while(arg[i] != NULL)
    {
        free(arg[i]);
        i = i + 1;
    }
    free(arg);
}
