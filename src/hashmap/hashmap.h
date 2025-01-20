#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Key structure
typedef struct {
    void* key;
    size_t key_size;
} Key;

// HashMap_Entry structure
typedef struct {
    Key key;
    void* value;
} HashMap_Entry;

// HashMap structure
typedef struct HashMap {
    // Variables
    HashMap_Entry* table; // Pointer to the actual data
    size_t capacity;      // Capacity of the hash table
    size_t size;          // Current number of entries in the hash table

    // Functions
    size_t (*hash_function)(const void* key,
                            size_t key_size); // Hash function for generic keys
    bool (*equals)(const void* a, const void* b,
                   size_t key_size); // Equality function for generic keys
    void (*insert)(struct HashMap* map, void* key, size_t key_size,
                   void* value); // Insert function for generic keys and values
    void* (*find)(
        struct HashMap* map, void* key,
        size_t key_size); // Find function for generic keys, returns void* value
    void (*delete_entry)(
        struct HashMap* map, void* key,
        size_t key_size); // Delete function for generic keys and values
} HashMap;

// Function prototypes
void hashmap_init(HashMap* map, size_t capacity);
void hashmap_cleanup(HashMap* map);
size_t hash_key(const void* key, size_t key_size);
bool key_equals(const void* a, const void* b, size_t key_size);
void insert(HashMap* map, void* key, size_t key_size, void* value);
void* find(HashMap* map, void* key, size_t key_size);
void delete_entry(HashMap* map, void* key, size_t key_size);

#endif
