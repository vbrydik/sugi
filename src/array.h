/*!
 * \file array.h
 * \brief this header file contains all defenitions for
 * array_t struct and functions associated with it.
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

struct array_t;

typedef struct array_t {
    void** array;
    uint32_t size;
    uint32_t used;
} array_t;

array_t* array_create_ref(uint32_t init_size);
array_t array_create(uint32_t init_size);
void array_resize(array_t* a);
void array_append(array_t* a, void* x);
void array_insert(array_t* a, void* x, int index);
void array_delete(array_t* a, int index);
int array_find(array_t* a, void* x);
void array_set(array_t* a, void* x, int index);
void* array_get(array_t* a, int index);
int array_len(array_t* a);
void array_print(array_t* a);

#endif // ARRAY_H
