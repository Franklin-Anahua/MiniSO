# MiniSO — Sistema Operativo

Estudiantes:
Franklin Cristofier Anahua Cahuachia		2024-119055
Ronald Alexander Huaman Huanca			    2024-119033
Jhon Williams Anahua Layme                  2021-119062
Docente:
Dr. HUGO MANUEL BARRAZA VIZCARRA
Semestre:  VI


## 1. Requisitos

Sistema operativo: Linux o UNIX con POSIX (no Windows nativo; usar WSL).

Compilador: GCC con C99 o superior.

Librería: POSIX Threads (pthread).

Opcional: ps y pstree (si falta pstree, el programa avisa y continúa).

## 2. Estructura del proyecto

```
Miniso/
├── main.cpp 
├── include/
│ ├── procesos.h 
│ ├── util.h
│ ├── hilos.h  
│ └── memoria.h 
├── src/
│ ├── procesos.c 
│ ├── hilos.c
│ └── memoria/
│     ├── particionamiento.c
│     └── paginacion.c
├── Makefile 
└── README.md 

---

## 3. Compilación

**Comando directo:**

gcc -std=c11 -D_GNU_SOURCE -Wall -Wextra -O2 -pthread \
    main.c procesos.c hilos.c particionamiento.c paginacion.c -o miniso



## 4. Ejecución


./miniso


Menú principal:

``` miniso  ```
1. gestion de procesos
2. gestion de hilos
3. gestion de memoria
0. salir
``` ```

## 5. Uso de la aplicación

### Módulo I — Procesos

Módulo I — Procesos
La opción 1 lanza hijos concurrentes con sleep, muestra el árbol con ps y pstree, y espera con wait.
La opción 2 muestra estados de finalización: crea hijos que salen con exit(42), mueren por señal y fallan en exec.
La opción 3 permite ejecutar un comando propio con fork, execvp y waitpid.

### Módulo II — Hilos

La opción 1 usa parámetros propios: pide capacidad del búfer, ítems por productor, número de productores y consumidores, y retardo en milisegundos. Muestra en tiempo real cada producción y consumo, y al final un resumen con producidos, consumidos, perdidos y duplicados.
La opción 2 ejecuta pruebas automáticas con tres configuraciones (búfer 1, 2 y 10) para verificar la sincronización.

### Módulo III — Memoria

**Particionamiento (first-fit):**

La opción 1 configura la memoria total.
La opción 2 solicita un bloque.
La opción 3 libera un bloque.
La opción 4 muestra el estado con mapa ASCII y estadísticas.
La opción 5 ejecuta un escenario de fragmentación externa.
La opción 6 ejecuta un escenario de fusión de huecos.


**Paginación:**

La opción 1 configura tamaño de página y marcos.
La opción 2 crea un proceso.
La opción 3 libera un proceso.
La opción 4 libera una página.
La opción 5 muestra los marcos.
La opción 6 muestra la tabla de páginas.
La opción 7 traduce una dirección lógica a física.
La opción 8 ejecuta una demo automática con casos inválidos.

---

## 6. Notas

- Entrada segura en `util.h` (`leer_entero`, `leer_opcion`); `Ctrl+D` sale limpiamente.
- El productor-consumidor usa mutex + variables de condición (`no_lleno`, `no_vacio`).
- Los módulos devuelven `0` en éxito y `-1` en error.

---

## 7. Problemas comunes

Si aparece "undefined reference to pthread_create", añadir -pthread.
Si aparece "pstree: command not found", instalar psmisc.
Si aparece "strsignal no declarado", compilar con -D_GNU_SOURCE.
Si no compila en Windows, usar WSL, MSYS2 o Linux.
