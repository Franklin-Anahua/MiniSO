#define _gnu_source

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "procesos.h"
#include "util.h"

#define max_args 16

static pid_t lanzar_hijo(const char *etiqueta, char *const argv[]) {

    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) {
        perror("error en fork");
        return -1;
    }
    if (pid == 0) {
        printf("\thijo %s: pid=%d ppid=%d, ejecuta %s\n",
                etiqueta, (int)getpid(), (int)getppid(), argv[0]);
        fflush(stdout);
        execvp(argv[0], argv);
        fprintf(stderr, "\thijo %s: pid=%d, error en exec %s: %s\n",
                etiqueta, (int)getpid(), argv[0], strerror(errno));
        _exit(127);
    }
    printf("\tpadre pid=%d creo al hijo %s con pid=%d\n",
           (int)getpid(), etiqueta, (int)pid);
    return pid;
}

static void informar_estado(pid_t pid, int st) {
    if (WIFEXITED(st)) {
        int c = WEXITSTATUS(st);
        printf("\thijo pid=%d termino normalmente con codigo %d%s\n",
               (int)pid, c, c == 127 ? " fallo de exec" : "");
    } else if (WIFSIGNALED(st)) {
        printf("\thijo pid=%d termino por la senal %d %s\n",
               (int)pid, WTERMSIG(st), strsignal(WTERMSIG(st)));
    } else {
        printf("\thijo pid=%d termino con estado 0x%x\n", (int)pid, st);
    }
}

static void esperar_hijos(int n) {
    int st;
    for (int i = 0; i < n; i++) {
        pid_t p = wait(&st);
        if (p < 0) {
            if (errno == EINTR) { i--; continue; }
            perror("error en wait");
            break;
        }
        informar_estado(p, st);
    }
}

static void mostrar_arbol(void) {
    char cmd[256];
    printf("\n  ps: hijos del proceso actual pid=%d\n", (int)getpid());
    snprintf(cmd, sizeof cmd, "ps -o pid,ppid,stat,etime,cmd --ppid %d", (int)getpid());
    if (system(cmd) == -1) perror("system ps");

    printf("\n  pstree -p %d\n", (int)getpid());
    snprintf(cmd, sizeof cmd,
             "pstree -p %d 2>/dev/null || echo '  pstree no instalado'",
             (int)getpid());
    if (system(cmd) == -1) perror("system pstree");
    printf("\n");
}

static void demo_hijos_concurrentes(void) {
    int n = leer_entero("  cantidad de hijos concurrentes", 1, 6);
    pid_t pids[6];
    char durs[6][8];
    char etiq[6][16];
    int creados = 0;

    printf("\n  padre pid=%d ppid=%d\n", (int)getpid(), (int)getppid());
    for (int i = 0; i < n; i++) {
        snprintf(durs[i], sizeof durs[i], "%d", 3 + 2 * i);
        snprintf(etiq[i], sizeof etiq[i], "h%d", i + 1);
        char *argv[] = {"sleep", durs[i], NULL};
        pids[i] = lanzar_hijo(etiq[i], argv);
        if (pids[i] > 0) creados++;
    }
    sleep(1);
    mostrar_arbol();
    printf("  padre: esperando %d hijos con wait\n", creados);
    esperar_hijos(creados);
}

static void demo_estados_y_errores(void) {
    int creados = 0;
    printf("\n  padre pid=%d. se crean 3 hijos: salida 42, muerte por senal, exec invalido\n",
           (int)getpid());

    fflush(stdout);
    pid_t a = fork();
    if (a < 0) perror("fork");
    else if (a == 0) {
        printf("  hijo a: pid=%d ppid=%d, termina con exit 42\n", (int)getpid(), (int)getppid());
        fflush(stdout);
        _exit(42);
    } else creados++;

    fflush(stdout);
    pid_t b = fork();
    if (b < 0) perror("fork");
    else if (b == 0) {
        printf("  hijo b: pid=%d ppid=%d, se envia sigterm a si mismo\n",
               (int)getpid(), (int)getppid());
        fflush(stdout);
        kill(getpid(), SIGTERM);
        _exit(0);
    } else creados++;

    char *argv_mal[] = {"programa_que_no_existe", NULL};
    if (lanzar_hijo("c", argv_mal) > 0) creados++;

    esperar_hijos(creados);
}

static void ejecutar_comando_usuario(void) {
    char linea[256];
    char *argv[max_args];
    int n = 0;

    printf("  comando a ejecutar ejemplo ls -l /tmp: ");
    fflush(stdout);
    if (!fgets(linea, sizeof linea, stdin)) return;

    char *tok = strtok(linea, " \t\n");
    while (tok && n < max_args - 1) {
        argv[n++] = tok;
        tok = strtok(NULL, " \t\n");
    }
    argv[n] = NULL;
    if (n == 0) { printf("  >> comando vacio\n"); return; }

    pid_t pid = lanzar_hijo("usr", argv);
    if (pid < 0) return;

    printf("  padre: esperando con waitpid %d. puede abrir otra terminal y usar ps o pstree\n",
           (int)pid);
    int st;
    if (waitpid(pid, &st, 0) < 0) { perror("error en waitpid"); return; }
    informar_estado(pid, st);
}

void modulo_procesos(void) {
    int op;
    do {
        printf("\n----------------------------------------\n");
        printf("\t modulo i - gestion de procesos\n");
        printf("----------------------------------------\n");
        printf("  1. hijos concurrentes con ps pstree y wait\n");
        printf("  2. estados de finalizacion y errores de exec\n");
        printf("  3. ejecutar un comando propio fork exec waitpid\n");
        printf("  0. volver al menu principal\n");
        op = leer_opcion("opcion");
        if (op == 1)      demo_hijos_concurrentes();
        else if (op == 2) demo_estados_y_errores();
        else if (op == 3) ejecutar_comando_usuario();
        if (op != 0) pausar();
    } while (op != 0);
}
