# MiniSO - Sistema Operativo

## 1. Requisitos

Para compilar y ejecutar el proyecto se necesitan los siguientes requisitos:

- Sistema operativo Linux o UNIX compatible con POSIX.
- Compilador GCC.
- Lenguajes de programación C y C++.
- Biblioteca POSIX Threads (`pthread`).
- Herramientas opcionales: `ps` y `pstree`.

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

## 3. Compilación y ejecución

Para compilar el proyecto, ejecutar el siguiente comando en la terminal:

```bash
make
```

Una vez finalizada la compilación, iniciar la aplicación con:

```bash
./miniso
```

## 4. Videos de demostración

### 4.1. Franklin Cristofier Anahua Cahuachia

- **Video** 

### 4.2. Ronald Alexander Huaman Huanca

- **Video**

### 4.3. Jhon Williams Anahua Layme

- **Video**

### 5. Archivos y documentación
- Informe final del proyecto: https://docs.google.com/document/d/1cflZgP408fFnBDhgdkAieYE9boV1dYxZhcoZCB1YRjY/edit?usp=sharing
- - Diapositivas de presentación: https://canva.link/dpucvxk4e31804x
