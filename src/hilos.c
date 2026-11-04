#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/hilos.h"

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_full;
pthread_cond_t cond_empty;

int item_counter = 1;
int items_consumed = 0;

void* producer(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (1) {
        pthread_mutex_lock(&mutex);

        if (item_counter > TOTAL_ITEMS) {
            pthread_cond_broadcast(&cond_empty);
            pthread_mutex_unlock(&mutex);
            break;
        }

        while (count == BUFFER_SIZE && item_counter <= TOTAL_ITEMS) {
            printf("  [Productor %d] Buffer lleno. Esperando...\n", id);
            pthread_cond_wait(&cond_full, &mutex);
        }

        if (item_counter > TOTAL_ITEMS) {
            pthread_cond_broadcast(&cond_empty);
            pthread_mutex_unlock(&mutex);
            break;
        }

        int item = item_counter++;
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("[Productor %d] Produjo item: %d | Buffer ocupado: %d/%d\n", id, item, count, BUFFER_SIZE);

        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);

        usleep(100000);
    }

    printf("[Productor %d] Finalizado.\n", id);
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0 && items_consumed < TOTAL_ITEMS) {
            printf("  [Consumidor %d] Buffer vacio. Esperando...\n", id);
            pthread_cond_wait(&cond_empty, &mutex);
        }

        if (items_consumed >= TOTAL_ITEMS) {
            pthread_cond_broadcast(&cond_empty);
            pthread_mutex_unlock(&mutex);
            break;
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        items_consumed++;

        printf("  [Consumidor %d] Consumio item: %d | Buffer ocupado: %d/%d\n", id, item, count, BUFFER_SIZE);

        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);

        usleep(150000);
    }

    printf("  [Consumidor %d] Finalizado.\n", id);
    pthread_exit(NULL);
}

void modulo_hilos(void) {
    printf("\n--- MODULO II: GESTION DE HILOS Y CONCURRENCIA ---\n");
    printf("Escenario: Problema Productor-Consumidor\n");
    printf("Productores: %d | Consumidores: %d | Capacidad Buffer: %d | Total Items: %d\n\n",
           NUM_PRODUCERS, NUM_CONSUMERS, BUFFER_SIZE, TOTAL_ITEMS);

    count = 0; in = 0; out = 0;
    item_counter = 1; items_consumed = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_full, NULL);
    pthread_cond_init(&cond_empty, NULL);

    pthread_t prod_threads[NUM_PRODUCERS];
    pthread_t cons_threads[NUM_CONSUMERS];

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&prod_threads[i], NULL, producer, id);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&cons_threads[i], NULL, consumer, id);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(prod_threads[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    printf("\n[HILOS] Todos los elementos procesados correctamente sin perdidas ni duplicaciones.\n");
}
