#include <stdio.h>
#include "util.h"
#include "procesos.h"
#include "hilos.h"
#include "memoria.h"

static void menu_memoria(void) {
    int op;
    do {
        printf("\n----------------------------------------\n");
        printf("\tmodulo iii gestion de memoria\n");
        printf("----------------------------------------\n");
        printf("  1. particionamiento de memoria first-fit\n");
        printf("  2. paginacion\n");
        printf("  0. volver\n");
        op = leer_opcion("opcion");
        if (op == 1)      menu_particionamiento();
        else if (op == 2) menu_paginacion();
    } while (op != 0);
}

int main(void) {
    int op;

    printf("\nminiso\n\n");

    do {
        printf("----------------------------------------\n");
        printf(" menu principal\n");
        printf("----------------------------------------\n");
        printf("  1. gestion de procesos\n");
        printf("  2. gestion de hilos\n");
        printf("  3. gestion de memoria\n");
        printf("  0. salir\n");

        op = leer_opcion("opcion");

        switch (op) {
            case 1: modulo_procesos(); break;
            case 2: modulo_hilos();    break;
            case 3: menu_memoria();    break;
            default: break;
        }

    } while (op != 0);

    printf("hasta luego\n");
    return 0;
}
