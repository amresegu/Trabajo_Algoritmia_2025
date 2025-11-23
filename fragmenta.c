#include "fragmenta.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char **fragmenta(const char *e) {

    char *cadena = strdup(e);
    if (cadena == NULL) {
        fprintf(stderr, "ERROR: no se pudo duplicar la cadena en fragmenta\n");
        exit(EXIT_FAILURE);
    }
    
    int tamano = 0;
    char *s_temp = strdup(e);
    if (s_temp == NULL) {
        fprintf(stderr, "ERROR: no se pudo duplicar la cadena en fragmenta\n");
        free(cadena);
        exit(EXIT_FAILURE);
    }

    char *tok = strtok(s_temp, ",\r\n");
    while (tok != NULL) {
        tamano++;
        tok = strtok(NULL, ",\r\n");
    }
    free(s_temp);

    char **salida = malloc((tamano + 1) * sizeof(char *));
    if (salida == NULL) {
        fprintf(stderr, "ERROR: no se pudo reservar memoria en fragmenta\n");
        free(cadena);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    tok = strtok(cadena, ",\r\n");

    while (tok != NULL) {
        salida[i] = strdup(tok);
        if (salida[i] == NULL) {
            fprintf(stderr, "ERROR: no se pudo duplicar token en fragmenta\n");

            // liberar lo ya reservado
            for (int j = 0; j < i; j++) {
                free(salida[j]);
            }
            free(salida);
            free(cadena);
            exit(EXIT_FAILURE);
        }

        i++;
        tok = strtok(NULL, ",\r\n");
    }

    salida[i] = NULL;  

    free(cadena);
    return salida;
}

void borrarg(char **arg)
{
    if (!arg) return;

    int i = 0;
    while (arg[i] != NULL) {
        free(arg[i]);
        i++;
    }

    free(arg);
}
