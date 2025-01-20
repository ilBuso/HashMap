#include "hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FNV-1a (Fowler–Noll–Vo)
uint32_t fnv1a(const void* key, size_t len) {
    const uint8_t* data = (const uint8_t*)key;
    uint32_t hash = 2166136261u;

    for (size_t i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= 16777619u;
    }

    return hash;
}

// Hash function
size_t hash_key(const void* key, size_t key_size) {
    return fnv1a(key, key_size);
}

// Equality function
bool key_equals(const void* a, const void* b, size_t key_size) {
    return memcmp(a, b, key_size) == 0;
}

// Initializes the hash map with a specified capacity
void hashmap_init(HashMap* map, size_t capacity) {
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(capacity * sizeof(HashMap_Entry));

    // Initialize table to NULL
    for (size_t i = 0; i < capacity; ++i) {
        ((HashMap_Entry*)map->table)[i].key.key = NULL;
    }

    // Set the function pointers for the operations
    map->hash_function = hash_key;
    map->equals = key_equals;
    map->insert = insert;
    map->find = find;
    map->delete_entry = delete_entry;
}

// Cleans up the hash map
void hashmap_cleanup(HashMap* map) {
    if (map->table) {
        free(map->table);
    }
}

// Insert a key-value pair into the hash map
void insert(HashMap* map, void* key, size_t key_size, void* value) {
    if (map->size == map->capacity) {
        printf("[ERROR] HashMap is full\n");
        return;
    }

    size_t index = map->hash_function(key, key_size) % map->capacity;

    // Search for the next available spot
    while (((HashMap_Entry*)map->table)[index].key.key != NULL) {
        // Check if the key already exists
        if (map->equals(((HashMap_Entry*)map->table)[index].key.key, key,
                        key_size)) {
            // Update the value if key already exists
            ((HashMap_Entry*)map->table)[index].value = value;
            return;
        }
        // Next index
        index = (index + 1) % map->capacity;
    }

    // Insert entry
    ((HashMap_Entry*)map->table)[index].key.key = malloc(key_size);
    // Copy the key
    memcpy(((HashMap_Entry*)map->table)[index].key.key, key, key_size);
    // Set the key size
    ((HashMap_Entry*)map->table)[index].key.key_size = key_size;
    // Set the value
    ((HashMap_Entry*)map->table)[index].value = value;
    // Increase size
    map->size++;
}

// Find a value by key in the hash map
void* find(HashMap* map, void* key, size_t key_size) {
    size_t index = map->hash_function(key, key_size) % map->capacity;

    // Find the correct key
    while (((HashMap_Entry*)map->table)[index].key.key != NULL) {
        if (map->equals(((HashMap_Entry*)map->table)[index].key.key, key,
                        key_size)) {
            // Return the value if found
            return ((HashMap_Entry*)map->table)[index].value;
        }
        // Next index
        index = (index + 1) % map->capacity;
    }

    // Return NULL if the key is not found
    return NULL;
}

// Delete an entry from the hash map by key
void delete_entry(HashMap* map, void* key, size_t key_size) {
    size_t index = map->hash_function(key, key_size) % map->capacity;

    // Find the correct key
    while (((HashMap_Entry*)map->table)[index].key.key != NULL) {
        if (map->equals(((HashMap_Entry*)map->table)[index].key.key, key,
                        key_size)) {
            // Free the key memory
            free(((HashMap_Entry*)map->table)[index].key.key);
            // Set key to NULL
            ((HashMap_Entry*)map->table)[index].key.key = NULL;
            // Decrease Size
            map->size--;
            return;
        }
        // Next index
        index = (index + 1) % map->capacity;
    }

    printf("[ERROR] Key not found\n");
}
