#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "hash.h"


void ht_free(hash_table_t* ht)
{
    free(ht->values);
    free(ht->keys);
    // array_free(ht->keys_array);
    ht = NULL;
    free(ht);
}


uint32_t hash_function(hash_table_t* ht, char* key)
{
    uint32_t hash = 0;
    for (int i = 0; i < (int)strlen(key); i++) {
        int x = (int)key[i];
        for (int j = 0; j < i; j++)
            x *= x;
        hash += x;
    }
    return hash % ht->size;
}


void ht_init_internal(hash_table_t* ht, uint32_t size)
{
    ht->size = size;
    ht->values = (void **)malloc(sizeof(void *) * size);
    ht->keys = (char **)malloc(sizeof(char *) * size);
    ht->k_array = array_create(size);
    for (uint32_t i = 0; i < size; i++)
    {
        ht->values[i] = NULL;
        ht->keys[i] = NULL;
    }
}


hash_table_t* ht_create_ref(uint32_t size)
{
    hash_table_t* ht = malloc(sizeof (hash_table_t));
    ht_init_internal(ht, size);
    return ht;
}


hash_table_t ht_create(uint32_t size)
{
    hash_table_t ht;
    ht_init_internal(&ht, size);
    return ht;
}


int ht_handle_insert_collision(hash_table_t* ht, char* key, uint32_t* hash)
{
    uint32_t h = *hash;
    bool found = false;
    for (uint32_t i = 0; i < ht->size; i++)
    {
        h = (h + 1) % ht->size;
        if (key == ht->keys[h]) {
            printf("KeyError: (%s == %s)\n", key, ht->keys[h]);
            return 0;
        }
        else if (ht->keys[h] == NULL)
        {
            printf("New hash for '%s': %u\n", key, h);
            found = true;
            break;
        }
    }
    if (!found)
        return 0;

    *hash = h;
    return 1;
}


int ht_insert(hash_table_t* ht, char* key, void* value_ptr)
{
    uint32_t hash = hash_function(ht, key);

    // Check for space in hash table
    if (ht->count >= ht->size)
    {
        printf("No space left in hash table!\n");
        return 0;
    }
    // Check for a same key
    else if (key == ht->keys[hash])
    {
        printf("KeyError: (%s == %s)\n", key, ht->keys[hash]);
        return 0;
    }
    // Collision, try to find an empty cell
    else if (ht->keys[hash] != NULL)
    {
        if (!ht_handle_insert_collision(ht, key, &hash))
            return 0;
    }

    ht->keys[hash] = key;
    ht->values[hash] = value_ptr;
    array_append(&ht->k_array, (void*)key);
    ht->count++;
    return 1;
}


int ht_handle_write_collision(hash_table_t* ht, char* key, uint32_t* hash)
{
    uint32_t h = *hash;
    bool found = false;
    for (uint32_t i = 0; i < ht->size; i++)
    {
        h = (h + 1) % ht->size;
        if (key == ht->keys[h]) {
            found = true;
            break;
        }
    }
    if (!found)
        return 0;

    *hash = h;
    return 1;
}


int ht_write(hash_table_t* ht, char* key, void* value_ptr)
{
    if (ht_has_key(ht, key))
    {
        uint32_t hash = hash_function(ht, key);

        if (ht->keys[hash] != key)
        {
            if (!ht_handle_write_collision(ht, key, &hash))
                return 0;
        }

        ht->values[hash] = value_ptr;
        return 1;
    }
    return 0;
}


int ht_delete(hash_table_t* ht, char* key)
{
    bool found = false;
    uint32_t hash = hash_function(ht, key);
    if (ht->keys[hash] != NULL)
    {
        if (key == ht->keys[hash])
        {
            found = true;
        }
        else
        {
            for (uint32_t i = 0; i < ht->size; i++)
            {
                hash = (hash + 1) % ht->size;
                if (key == ht->keys[hash])
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            char* key = ht->keys[hash];
            int i = array_find(&ht->k_array, (void*)key);
            array_delete(&ht->k_array, i);

            ht->keys[hash] = NULL;
            ht->values[hash] = NULL;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    return 0;
}


int ht_has_key(hash_table_t* ht, char* key)
{
    return (array_find(&ht->k_array, key) < 0) ? 0 : 1;
}


void* ht_read(hash_table_t* ht, char* key)
{
    uint32_t hash = hash_function(ht, key);
    if (ht->keys[hash] == NULL)
    {
        return NULL;
    }
    else
    {
        if (ht->keys[hash] == key)
        {
            return ht->values[hash];
        }
        else
        {
            for (uint32_t i = 0; i < ht->size; i++)
            {
                hash = (hash + 1) % ht->size;
                if (ht->keys[hash] == key)
                    return ht->values[hash];
            }
            return NULL;
        }
    }
}


void ht_print(hash_table_t* ht)
{
    printf("<hash_table_t %p> {\n", ht);
    for (uint32_t i = 0; i < ht->size; i++)
    {
        if (ht->keys[i] != NULL)
        {
            char* key = ht->keys[i];
            void* value = ht->values[i];
            printf("  [%u] %s: %p\n", i, key, value);
        }
    }
    printf("}\n");
}
