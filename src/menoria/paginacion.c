#include <stdio.h>
#include <string.h>
#include "memoria.h"
#include "util.h"

#define max_marcos 256
#define max_proc   16
#define max_pag    64

typedef struct {
    int activo;
    int pid;
    int bytes;
    int npag;
    int tabla[max_pag];
} proceso;

static int tam_pag = 0, n_marcos = 0;
static int marco_pid[max_marcos];
static int marco_pag[max_marcos];
static proceso proc[max_proc];

void pag_configurar(int tam, int marcos) {
    tam_pag = tam;
    n_marcos = marcos;
    for (int i = 0; i < max_marcos; i++) { marco_pid[i] = 0; marco_pag[i] = -1; }
    memset(proc, 0, sizeof proc);
    for (int i = 0; i < max_proc; i++)
        for (int j = 0; j < max_pag; j++)
            proc[i].tabla[j] = -1;
    printf(">> paginacion configurada pagina %d bytes marcos %d memoria fisica %d bytes\n",
           tam, marcos, tam * marcos);
}

static proceso *buscar_proc(int pid) {
    for (int i = 0; i < max_proc; i++)
        if (proc[i].activo && proc[i].pid == pid) return &proc[i];
    return NULL;
}

int pag_crear_proceso(int pid, int bytes) {
    if (tam_pag == 0) { printf(">> primero configure la paginacion\n"); return -1; }
    if (pid <= 0 || bytes <= 0) { printf(">> pid y tamano deben ser positivos\n"); return -1; }
    if (buscar_proc(pid)) { printf(">> el proceso %d ya existe\n", pid); return -1; }

    int npag = (bytes + tam_pag - 1) / tam_pag;
    if (npag > max_pag) { printf(">> el proceso excede %d paginas\n", max_pag); return -1; }

    int libres = 0;
    for (int i = 0; i < n_marcos; i++) if (marco_pid[i] == 0) libres++;
    if (libres < npag) {
        printf(">> proceso %d necesita %d marcos y solo hay %d libres\n", pid, npag, libres);
        return -1;
    }
    proceso *p = NULL;
    for (int i = 0; i < max_proc; i++) if (!proc[i].activo) { p = &proc[i]; break; }
    if (!p) { printf(">> tabla de procesos llena\n"); return -1; }

    p->activo = 1; p->pid = pid; p->bytes = bytes; p->npag = npag;
    for (int i = 0; i < max_pag; i++) p->tabla[i] = -1;

    int pg = 0;
    for (int m = 0; m < n_marcos && pg < npag; m++) {
        if (marco_pid[m] == 0) {
            marco_pid[m] = pid;
            marco_pag[m] = pg;
            p->tabla[pg++] = m;
        }
    }
    printf(">> proceso %d con %d bytes usa %d paginas fragmentacion interna final %d bytes\n",
           pid, bytes, npag, npag * tam_pag - bytes);
    return 0;
}

int pag_liberar_proceso(int pid) {
    proceso *p = buscar_proc(pid);
    if (!p) { printf(">> el proceso %d no existe\n", pid); return -1; }
    for (int i = 0; i < n_marcos; i++)
        if (marco_pid[i] == pid) { marco_pid[i] = 0; marco_pag[i] = -1; }
    p->activo = 0;
    printf(">> proceso %d liberado sus marcos quedaron libres\n", pid);
    return 0;
}

int pag_liberar_pagina(int pid, int pagina) {
    proceso *p = buscar_proc(pid);
    if (!p) { printf(">> el proceso %d no existe\n", pid); return -1; }
    if (pagina < 0 || pagina >= p->npag || p->tabla[pagina] < 0) {
        printf(">> la pagina %d del proceso %d no esta asignada\n", pagina, pid);
        return -1;
    }
    int m = p->tabla[pagina];
    marco_pid[m] = 0; marco_pag[m] = -1; p->tabla[pagina] = -1;
    printf(">> pagina %d del proceso %d liberada el marco %d quedo libre\n", pagina, pid, m);
    return 0;
}

void pag_mostrar_marcos(void) {
    if (tam_pag == 0) { printf(">> paginacion sin configurar\n"); return; }
    printf("\n  marco estado\n  -----------------------\n");
    for (int i = 0; i < n_marcos; i++) {
        if (marco_pid[i]) printf("  %5d proceso %d pagina %d\n", i, marco_pid[i], marco_pag[i]);
        else              printf("  %5d libre\n", i);
    }
}

void pag_mostrar_tabla(int pid) {
    proceso *p = buscar_proc(pid);
    if (!p) { printf(">> el proceso %d no existe\n", pid); return; }
    printf("\n  tabla de paginas del proceso %d con %d bytes y %d paginas\n", pid, p->bytes, p->npag);
    printf("  pagina marco\n  -------------\n");
    for (int i = 0; i < p->npag; i++) {
        if (p->tabla[i] >= 0) printf("  %6d %d\n", i, p->tabla[i]);
        else                  printf("  %6d sin asignar\n", i);
    }
}

int pag_traducir(int pid, int dir, long *fisica) {
    proceso *p = buscar_proc(pid);
    if (!p) { printf(">> el proceso %d no existe\n", pid); return -1; }
    if (dir < 0 || dir >= p->bytes) {
        printf(">> direccion invalida %d fuera del espacio logico del proceso %d de 0 a %d\n",
               dir, pid, p->bytes - 1);
        return -1;
    }
    int pagina = dir / tam_pag;
    int desp   = dir % tam_pag;
    printf("  direccion logica %d pagina %d entre %d da %d desplazamiento %d modulo %d da %d\n",
           dir, dir, tam_pag, pagina, dir, tam_pag, desp);
    if (p->tabla[pagina] < 0) {
        printf("  >> fallo de pagina la pagina %d del proceso %d no tiene marco\n",
               pagina, pid);
        return -1;
    }
    int marco = p->tabla[pagina];
    long fis = (long)marco * tam_pag + desp;
    printf("  tabla pagina %d va al marco %d direccion fisica %d por %d mas %d da %ld\n",
           pagina, marco, marco, tam_pag, desp, fis);
    if (fisica) *fisica = fis;
    return 0;
}

static void demo_paginacion(void) {
    printf("\ndemo de paginacion con pagina 256 bytes y 8 marcos\n");
    pag_configurar(256, 8);
    pag_crear_proceso(10, 512);
    pag_crear_proceso(20, 1024);
    printf("\nproceso 30 con 768 bytes son 3 paginas no cabe solo quedan 2 marcos\n");
    pag_crear_proceso(30, 768);
    printf("\nse libera el proceso 10 y se reintenta el 30 usara marcos no contiguos\n");
    pag_liberar_proceso(10);
    pag_crear_proceso(30, 768);
    pag_mostrar_marcos();
    pag_mostrar_tabla(20);
    pag_mostrar_tabla(30);

    printf("\ntraducciones contrastadas con calculo manual\n");
    struct { int pid, dir; long esperado; } t[] = {
        {20,  300,  812},
        {20, 1000, 1512},
        {30,  700, 1724},
    };
    for (int i = 0; i < 3; i++) {
        long f = -1;
        printf("\nproceso %d\n", t[i].pid);
        pag_traducir(t[i].pid, t[i].dir, &f);
        printf("manual %ld simulador %ld resultado %s\n", t[i].esperado, f,
               f == t[i].esperado ? "coincide" : "difiere");
    }

    printf("\ncasos excepcionales\n");
    printf("\nproceso 20 direccion 5000 fuera de rango\n");
    pag_traducir(20, 5000, NULL);
    printf("\nse libera la pagina 1 del proceso 30 y se traduce la direccion 300\n");
    pag_liberar_pagina(30, 1);
    pag_traducir(30, 300, NULL);
}

void menu_paginacion(void) {
    int op;
    if (tam_pag == 0) pag_configurar(256, 8);
    do {
        printf("\n----------------------------------------\n");
        printf(" modulo iii-b paginacion\n");
        printf("----------------------------------------\n");
        printf("  1. configurar tamano de pagina y marcos reinicia\n");
        printf("  2. crear proceso y asignar paginas a marcos\n");
        printf("  3. liberar proceso\n");
        printf("  4. liberar una pagina de un proceso\n");
        printf("  5. mostrar marcos de memoria fisica\n");
        printf("  6. mostrar tabla de paginas de un proceso\n");
        printf("  7. traducir direccion logica a fisica\n");
        printf("  8. demo con traducciones y casos invalidos\n");
        printf("  0. volver\n");
        op = leer_opcion("opcion");
        switch (op) {
            case 1: {
                int t = leer_entero("  tamano de pagina en bytes", 1, 100000);
                int m = leer_entero("  cantidad de marcos", 1, max_marcos);
                pag_configurar(t, m);
                break;
            }
            case 2: {
                int pid = leer_entero("  pid del proceso", 1, 100000);
                int b = leer_entero("  tamano del proceso en bytes", 1, 10000000);
                if (pag_crear_proceso(pid, b) == 0) pag_mostrar_tabla(pid);
                break;
            }
            case 3: pag_liberar_proceso(leer_entero("  pid", 1, 100000)); break;
            case 4: {
                int pid = leer_entero("  pid", 1, 100000);
                int pg = leer_entero("  numero de pagina", 0, max_pag - 1);
                pag_liberar_pagina(pid, pg);
                break;
            }
            case 5: pag_mostrar_marcos(); break;
            case 6: pag_mostrar_tabla(leer_entero("  pid", 1, 100000)); break;
            case 7: {
                int pid = leer_entero("  pid", 1, 100000);
                int d = leer_entero("  direccion logica", 0, 10000000);
                pag_traducir(pid, d, NULL);
                break;
            }
            case 8: demo_paginacion(); break;
            default: break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}
