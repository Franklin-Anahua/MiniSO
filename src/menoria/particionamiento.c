#include <stdio.h>
#include <string.h>
#include "memoria.h"
#include "util.h"

#define max_bloques 128

typedef struct {
    int inicio;
    int tam;
    int id;
} bloque;

static bloque bl[max_bloques];
static int nb = 0;
static int mem_total = 0;

void part_configurar(int total) {
    mem_total = total;
    nb = 1;
    memset(bl, 0, sizeof bl);
    bl[0].inicio = 0;
    bl[0].tam = total;
    bl[0].id = 0;
    printf(">> memoria configurada con %d kb en un unico bloque libre\n", total);
}

static int buscar_id(int id) {
    for (int i = 0; i < nb; i++)
        if (bl[i].id == id) return i;
    return -1;
}

static void estadisticas(int *libre, int *mayor, int *huecos) {
    *libre = *mayor = *huecos = 0;
    for (int i = 0; i < nb; i++) {
        if (bl[i].id == 0) {
            *libre += bl[i].tam;
            (*huecos)++;
            if (bl[i].tam > *mayor) *mayor = bl[i].tam;
        }
    }
}

int part_solicitar(int id, int tam) {
    if (mem_total == 0) { printf(">> primero configure la memoria\n"); return -1; }
    if (id <= 0 || tam <= 0) { printf(">> id y tamano deben ser positivos\n"); return -1; }
    if (buscar_id(id) >= 0) { printf(">> el proceso %d ya tiene un bloque asignado\n", id); return -1; }

    for (int i = 0; i < nb; i++) {
        if (bl[i].id == 0 && bl[i].tam >= tam) {
            if (bl[i].tam > tam) {
                if (nb >= max_bloques) { printf(">> tabla de bloques llena\n"); return -1; }
                memmove(&bl[i + 2], &bl[i + 1], (nb - i - 1) * sizeof(bloque));
                bl[i + 1].inicio = bl[i].inicio + tam;
                bl[i + 1].tam = bl[i].tam - tam;
                bl[i + 1].id = 0;
                nb++;
                bl[i].tam = tam;
            }
            bl[i].id = id;
            printf(">> first-fit proceso %d con %d kb va al bloque %d a %d\n",
                   id, tam, bl[i].inicio, bl[i].inicio + bl[i].tam - 1);
            return bl[i].inicio;
        }
    }

    int libre, mayor, huecos;
    estadisticas(&libre, &mayor, &huecos);
    if (libre >= tam)
        printf(">> proceso %d con %d kb rechazado por fragmentacion externa hay %d kb en %d huecos mayor %d kb\n",
               id, tam, libre, huecos, mayor);
    else
        printf(">> proceso %d con %d kb rechazado memoria insuficiente libre %d kb\n",
               id, tam, libre);
    return -1;
}

int part_liberar(int id) {
    int i = buscar_id(id);
    if (i < 0) { printf(">> el proceso %d no tiene bloque asignado\n", id); return -1; }
    bl[i].id = 0;
    printf(">> proceso %d liberado del bloque %d a %d\n",
           id, bl[i].inicio, bl[i].inicio + bl[i].tam - 1);

    for (int k = 0; k + 1 < nb; ) {
        if (bl[k].id == 0 && bl[k + 1].id == 0) {
            printf(">> union de huecos %d a %d con %d a %d\n",
                   bl[k].inicio, bl[k].inicio + bl[k].tam - 1,
                   bl[k + 1].inicio, bl[k + 1].inicio + bl[k + 1].tam - 1);
            bl[k].tam += bl[k + 1].tam;
            memmove(&bl[k + 1], &bl[k + 2], (nb - k - 2) * sizeof(bloque));
            nb--;
        } else k++;
    }
    return 0;
}

void part_mostrar(void) {
    if (mem_total == 0) { printf(">> memoria sin configurar\n"); return; }
    printf("\n  numero inicio fin tamano estado\n");
    printf("  --------------------------------\n");
    for (int i = 0; i < nb; i++) {
        printf("  %-3d %6d %6d %7d  ", i, bl[i].inicio, bl[i].inicio + bl[i].tam - 1, bl[i].tam);
        if (bl[i].id) printf("ocupado por proceso %d\n", bl[i].id);
        else          printf("libre\n");
    }
    printf("\n  [");
    for (int i = 0; i < nb; i++) {
        int w = bl[i].tam * 60 / mem_total;
        if (w < 1) w = 1;
        for (int k = 0; k < w; k++) putchar(bl[i].id ? '#' : '.');
        putchar('|');
    }
    printf("]\n");

    int libre, mayor, huecos;
    estadisticas(&libre, &mayor, &huecos);
    printf("\n  total %d kb ocupado %d kb libre %d kb huecos %d mayor hueco %d kb\n",
           mem_total, mem_total - libre, libre, huecos, mayor);
    if (huecos > 1)
        printf("  fragmentacion externa %d kb dispersos en %d huecos asignable maximo %d kb contiguos\n",
               libre, huecos, mayor);
    else
        printf("  sin fragmentacion externa\n");
}

static void escenario_1(void) {
    subtitulo("escenario 1 fragmentacion externa total 100 kb");
    part_configurar(100);
    part_solicitar(1, 20); part_solicitar(2, 30);
    part_solicitar(3, 10); part_solicitar(4, 25);
    part_mostrar();
    printf("\n  >> se liberan los procesos 2 y 4\n");
    part_liberar(2); part_liberar(4);
    part_mostrar();
    printf("\n  >> el proceso 5 pide 50 kb hay 70 libres pero fragmentados\n");
    part_solicitar(5, 50);
    printf("\n  >> el proceso 6 pide 25 kb first-fit reutiliza el primer hueco que quepa\n");
    part_solicitar(6, 25);
    part_mostrar();
}

static void escenario_2(void) {
    subtitulo("escenario 2 reutilizacion y union de huecos total 64 kb");
    part_configurar(64);
    part_solicitar(1, 16); part_solicitar(2, 16);
    part_solicitar(3, 16); part_solicitar(4, 16);
    part_mostrar();
    printf("\n  >> se liberan 1 y 3 los huecos quedan separados por el 2\n");
    part_liberar(1); part_liberar(3);
    part_mostrar();
    printf("\n  >> el proceso 5 pide 20 kb no cabe en ningun hueco de 16 kb\n");
    part_solicitar(5, 20);
    printf("\n  >> se libera el 2 los tres bloques contiguos se unen en 48 kb\n");
    part_liberar(2);
    part_mostrar();
    printf("\n  >> el proceso 5 pide 20 kb nuevamente\n");
    part_solicitar(5, 20);
    part_mostrar();
}

void menu_particionamiento(void) {
    int op;
    if (mem_total == 0) part_configurar(100);
    do {
        printf("\n----------------------------------------\n");
        printf(" modulo iii-a particionamiento first-fit\n");
        printf("----------------------------------------\n");
        printf("  1. configurar memoria total reinicia todo\n");
        printf("  2. solicitar bloque\n");
        printf("  3. liberar bloque\n");
        printf("  4. mostrar estado de memoria\n");
        printf("  5. escenario 1 fragmentacion externa\n");
        printf("  6. escenario 2 reutilizacion y union de huecos\n");
        printf("  0. volver\n");
        op = leer_opcion("opcion");
        switch (op) {
            case 1:
                if (mem_total != 0) {
                    int conf = leer_entero("  confirmar reinicio 1 si 0 no", 0, 1);
                    if (!conf) break;
                }
                part_configurar(leer_entero("  memoria total en kb", 1, 100000));
                break;
            case 2: {
                int id = leer_entero("  id del proceso", 1, 100000);
                int t  = leer_entero("  tamano solicitado en kb", 1, 100000);
                part_solicitar(id, t);
                part_mostrar();
                break;
            }
            case 3: {
                int id = leer_entero("  id del proceso a liberar", 1, 100000);
                part_liberar(id);
                part_mostrar();
                break;
            }
            case 4: part_mostrar(); break;
            case 5: escenario_1(); break;
            case 6: escenario_2(); break;
            default: break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}