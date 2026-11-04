# MiniSO — Sistema Operativo

MiniSO es un proyecto académico desarrollado para demostrar el funcionamiento de los principales mecanismos de un sistema operativo: **gestión de procesos, gestión de hilos y gestión de memoria**.

## Información académica

**Estudiantes:**

| Apellidos y nombres | Código |
|---|---|
| Franklin Cristofier Anahua Cahuachia | 2024-119055 |
| Ronald Alexander Huaman Huanca | 2024-119033 |
| Jhon Williams Anahua Layme | 2021-119062 |

**Docente:** Dr. Hugo Manuel Barraza Vizcarra

**Semestre:** VI

---

## 1. Requisitos

Para compilar y ejecutar el proyecto se necesitan los siguientes requisitos:

- **Sistema operativo:** Linux o UNIX compatible con POSIX. En Windows se puede utilizar WSL.
- **Compilador:** GCC compatible con C99 o superior.
- **Biblioteca:** POSIX Threads (`pthread`).
- **Herramientas opcionales:** `ps` y `pstree` para visualizar los procesos. Si `pstree` no está instalado, el programa muestra un aviso y continúa con su ejecución.

## 2. Estructura del proyecto

```text
MiniSO/
├── main.cpp
├── include/
│   ├── procesos.h
│   ├── util.h
│   ├── hilos.h
│   └── memoria.h
├── src/
│   ├── procesos.c
│   ├── hilos.c
│   └── memoria/
│       ├── particionamiento.c
│       └── paginacion.c
├── Makefile
└── README.md
```

## 3. Compilación

El proyecto puede compilarse utilizando el `Makefile` o mediante un comando directo.

**Compilación con Makefile:**

```bash
make
```

**Compilación directa:**

```bash
gcc -std=c11 -D_GNU_SOURCE -Wall -Wextra -O2 -pthread \
    -Iinclude main.c procesos.c hilos.c particionamiento.c paginacion.c \
    -o miniso
```

## 4. Ejecución

Para iniciar la aplicación, ejecutar el siguiente comando:

```bash
./miniso
```

**Menú principal:**

```text
========== MiniSO ==========
1. Gestión de procesos
2. Gestión de hilos
3. Gestión de memoria
0. Salir
============================
```

## 5. Uso de la aplicación

### Módulo I — Gestión de procesos

Este módulo permite trabajar con la creación, ejecución y finalización de procesos mediante llamadas al sistema.

**Opciones disponibles:**

1. **Creación de procesos concurrentes:** crea procesos hijos que ejecutan operaciones con `sleep()`, muestra el árbol de procesos mediante `ps` y `pstree`, y espera su finalización utilizando `wait()`.
2. **Estados de finalización:** muestra diferentes estados de terminación de los procesos, incluyendo procesos que finalizan con `exit(42)`, procesos terminados por señales y errores durante la ejecución de `exec()`.
3. **Ejecución de comandos personalizados:** permite introducir y ejecutar comandos del sistema utilizando `fork()`, `execvp()` y `waitpid()`.

### Módulo II — Gestión de hilos

Este módulo implementa el problema clásico del productor-consumidor para demostrar la concurrencia y sincronización entre hilos.

**Opciones disponibles:**

1. **Configuración personalizada:** permite establecer la capacidad del búfer, la cantidad de elementos por productor, el número de productores y consumidores, y el tiempo de retardo en milisegundos.
2. **Ejecución en tiempo real:** muestra las operaciones de producción y consumo conforme se van realizando.
3. **Resumen de resultados:** presenta la cantidad de elementos producidos, consumidos, perdidos y duplicados.
4. **Pruebas automáticas:** ejecuta tres escenarios con diferentes capacidades de búfer (1, 2 y 10 elementos) para comprobar el funcionamiento de la sincronización.

La sincronización se realiza mediante mutex y variables de condición (`no_lleno` y `no_vacio`), evitando conflictos durante el acceso compartido al búfer.

### Módulo III — Gestión de memoria

Este módulo permite simular dos técnicas fundamentales de administración de memoria: particionamiento y paginación.

**A. Particionamiento (First Fit)**

Implementa el algoritmo First Fit para la asignación de bloques de memoria, seleccionando el primer espacio libre que tenga capacidad suficiente.

**Opciones disponibles:**

1. Configuración de la memoria total.
2. Solicitud y asignación de un bloque de memoria.
3. Liberación de un bloque asignado.
4. Visualización del estado de la memoria mediante un mapa ASCII y estadísticas.
5. Ejecución de un escenario de fragmentación externa.
6. Ejecución de un escenario de fusión de espacios libres adyacentes.

**B. Paginación**

Simula la administración de memoria mediante páginas y marcos, permitiendo gestionar procesos y realizar traducciones de direcciones lógicas a físicas.

**Opciones disponibles:**

1. Configuración del tamaño de página y cantidad de marcos.
2. Creación de procesos.
3. Liberación de procesos.
4. Liberación individual de páginas.
5. Visualización del estado de los marcos de memoria.
6. Visualización de la tabla de páginas.
7. Traducción de direcciones lógicas a direcciones físicas.
8. Ejecución de una demostración automática con diferentes casos, incluyendo direcciones y operaciones inválidas.

---

## 6. Notas

- **Entrada segura:** las funciones `leer_entero()` y `leer_opcion()`, implementadas en `util.h`, permiten controlar y validar las entradas del usuario.
- **Finalización segura:** al ingresar `Ctrl+D`, el programa reconoce el fin de entrada y permite salir limpiamente.
- **Sincronización de hilos:** el problema productor-consumidor utiliza mutex y variables de condición (`no_lleno` y `no_vacio`).
- **Manejo de errores:** los módulos utilizan `0` para indicar una operación exitosa y `-1` para indicar un error.

## 7. Problemas comunes

| Problema | Solución |
|---|---|
| `undefined reference to pthread_create` | Agregar `-pthread` al comando de compilación. |
| `pstree: command not found` | Instalar el paquete `psmisc`. |
| `strsignal undeclared` | Compilar utilizando `-D_GNU_SOURCE`. |
| Errores de compilación en Windows | Utilizar WSL o un entorno compatible con POSIX. |

Para instalar `pstree` en distribuciones Ubuntu o Debian:

```bash
sudo apt update
sudo apt install psmisc
```

---

