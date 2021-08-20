#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include "array.h"

struct hash_table_t;
typedef struct hash_table_t {
    char **keys;
    void **values;
    array_t k_array;
    uint32_t size;
    uint32_t count;
} hash_table_t;

uint32_t  hash_function(hash_table_t* ht, char* key);
hash_table_t  ht_create(uint32_t size);
hash_table_t* ht_create_ref(uint32_t size);
int   ht_insert(hash_table_t* ht, char* key, void* value_ptr);
int   ht_delete(hash_table_t* ht, char* key);
int   ht_write(hash_table_t* ht, char* key, void* value_ptr);
void* ht_read(hash_table_t* ht, char* key);
void  ht_free(hash_table_t* ht);
int   ht_has_key(hash_table_t* ht, char* key);
void  ht_print(hash_table_t* ht);

#endif // HASH_H
