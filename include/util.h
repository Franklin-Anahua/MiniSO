#ifndef util_h
#define util_h

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static inline int leer_entero(const char *msg, int min, int max) {
    char buf[64];
    char *fin;
    long v;
    for (;;) {
        printf("%s [%d-%d]: ", msg, min, max);
        fflush(stdout);
        if (!fgets(buf, sizeof buf, stdin)) {
            printf("\nentrada finalizada.\n");
            exit(0);
        }
        errno = 0;
        v = strtol(buf, &fin, 10);
        if (fin == buf || (*fin != '\n' && *fin != '\0') || errno || v < min || v > max) {
            printf("  >> valor invalido. debe ser un numero entre %d y %d.\n", min, max);
            continue;
        }
        return (int)v;
    }
}

static inline int leer_opcion(const char *msg) {
    char buf[64];
    char *fin;
    long v;
    for (;;) {
        printf("%s: ", msg);
        fflush(stdout);
        if (!fgets(buf, sizeof buf, stdin)) {
            printf("\nentrada finalizada.\n");
            exit(0);
        }
        errno = 0;
        v = strtol(buf, &fin, 10);
        if (fin == buf || (*fin != '\n' && *fin != '\0') || errno) {
            printf("  >> valor invalido. intente de nuevo.\n");
            continue;
        }
        return (int)v;
    }
}

static inline void pausar(void) {
    char buf[8];
    printf("\npresione enter para continuar");
    fflush(stdout);
    if (fgets(buf, sizeof buf, stdin)) { }
}

static inline void subtitulo(const char *t) {
    printf("\n--- %s ---\n", t);
}

#endif
