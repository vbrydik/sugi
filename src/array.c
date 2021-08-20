#include "array.h"

void array_init_internal(array_t* a, uint32_t init_size)
{
    a->array = (void **) malloc(sizeof (void*) * init_size);
    a->size = init_size;
    a->used = 0;
}

array_t* array_create_ref(uint32_t init_size)
{
    array_t* a = malloc(sizeof (array_t));
    array_init_internal(a, init_size);
    return a;
}

array_t array_create(uint32_t init_size)
{
    array_t a;
    array_init_internal(&a, init_size);
    return a;
}

void array_resize(array_t* a)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (void **) realloc(a->array, sizeof (void*) * a->size);
    }
}

void array_append(array_t* a, void* x)
{
    array_resize(a);

    a->array[a->used] = x;
    a->used++;
}

void array_insert(array_t* a, void* x, int index)
{
    array_resize(a);

    if ((a->used - index) >= 0)
    {
        memcpy(a->array + index + 1, a->array + index, sizeof (void*) * (a->used - index));
        a->array[index] = x;
        a->used++;
    }
}

void array_delete(array_t* a, int index)
{
    memcpy(a->array + index, a->array + index + 1, sizeof (void*) * (a->used - index - 1));
    a->used--;
}

int array_find(array_t* a, void* x)
{
    for (uint32_t i = 0; i < a->used; i++)
    {
        if (a->array[i] == x) {
            return i;
        }
    }
    return -1;
}


void array_set(array_t* a, void* x, int index)
{
    a->array[index] = x;
}

void* array_get(array_t* a, int index)
{
    return a->array[index];
}

int array_len(array_t* a)
{
    return a->used;
}

void array_print(array_t* a)
{
    printf("[\n");
    for (uint32_t i = 0; i < a->used; i++)
        printf("  %p\n", a->array[i]);
    printf("]\n");
}
