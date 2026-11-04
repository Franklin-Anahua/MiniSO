#ifndef memoria_h
#define memoria_h

void part_configurar(int total);
int  part_solicitar(int id, int tam);
int  part_liberar(int id);
void part_mostrar(void);
void menu_particionamiento(void);

void pag_configurar(int tam_pagina, int num_marcos);
int  pag_crear_proceso(int pid, int bytes);
int  pag_liberar_proceso(int pid);
int  pag_liberar_pagina(int pid, int pagina);
void pag_mostrar_marcos(void);
void pag_mostrar_tabla(int pid);
int  pag_traducir(int pid, int dir_logica, long *dir_fisica);
void menu_paginacion(void);

#endif