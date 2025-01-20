#include "hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief FNV-1a (Fowler–Noll–Vo) hash function.
 *
 * This function computes a hash value for a given key using the FNV-1a
 * algorithm. The FNV-1a hash function is a simple and fast hash function
 * commonly used for hash-based data structures like hash maps.
 *
 * @param key The key for which the hash value is to be computed.
 * @param len The length of the key in bytes.
 *
 * @return The computed hash value (a 32-bit unsigned integer).
 */
uint32_t fnv1a(const void* key, size_t len) {
    const uint8_t* data = (const uint8_t*)key;
    uint32_t hash = 2166136261u;

    for (size_t i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= 16777619u;
    }

    return hash;
}

/**
 * @brief Hash function to compute hash values for keys in the hash map.
 *
 * This function is used to compute the hash value for a given key. It calls the
 * `fnv1a` function to generate a hash value based on the key data and size.
 *
 * @param key The key for which the hash value is to be computed.
 * @param key_size The size of the key data in bytes.
 *
 * @return A computed hash value (size_t) used for index calculation in the hash
 * map.
 */
size_t hash_key(const void* key, size_t key_size) {
    return fnv1a(key, key_size);
}

/**
 * @brief Compares two keys for equality.
 *
 * This function compares two keys byte-by-byte using `memcmp`. If the keys are
 * identical, it returns `true`; otherwise, it returns `false`.
 *
 * @param a The first key to compare.
 * @param b The second key to compare.
 * @param key_size The size of the key data in bytes.
 *
 * @return `true` if the keys are equal, otherwise `false`.
 */
bool key_equals(const void* a, const void* b, size_t key_size) {
    return memcmp(a, b, key_size) == 0;
}

/**
 * @brief Initializes a hash map with a specified capacity.
 *
 * This function initializes the hash map, allocating memory for the hash table
 * (array of entries) based on the given capacity. The size of the hash map is
 * set to 0, and the table is filled with NULL pointers. The function also
 * sets the function pointers for the hash map operations.
 *
 * @param map A pointer to the hash map to initialize.
 * @param capacity The initial capacity of the hash map (number of slots in the
 * table).
 */
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

/**
 * @brief Cleans up the resources used by the hash map.
 *
 * This function frees the memory allocated for the hash map's table. It is
 * important to call this function when the hash map is no longer needed to
 * prevent memory leaks.
 *
 * @param map A pointer to the hash map to clean up.
 */
void hashmap_cleanup(HashMap* map) {
    if (map->table) {
        free(map->table);
    }
}

/**
 * @brief Inserts a key-value pair into the hash map.
 *
 * This function inserts a new key-value pair into the hash map. It checks if
 * the key already exists in the table. If so, it updates the value. If the
 * table is full, it prints an error message and does not insert the new pair.
 * The function handles collisions by using linear probing (i.e., trying the
 * next index when a collision is detected).
 *
 * @param map A pointer to the hash map to insert the key-value pair into.
 * @param key The key to insert.
 * @param key_size The size of the key data in bytes.
 * @param value The value associated with the key.
 */
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

/**
 * @brief Finds a value by key in the hash map.
 *
 * This function looks for a key in the hash map and returns the associated
 * value if found. If the key is not present in the table, it returns `NULL`. It
 * uses the hash function to locate the correct index in the table and handles
 * collisions by linear probing.
 *
 * @param map A pointer to the hash map to search in.
 * @param key The key to search for.
 * @param key_size The size of the key data in bytes.
 *
 * @return A pointer to the value associated with the key, or `NULL` if the key
 *         is not found.
 */
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

/**
 * @brief Deletes an entry from the hash map by key.
 *
 * This function removes a key-value pair from the hash map by searching for the
 * key and freeing the associated memory. If the key is not found, an error
 * message is printed. After deleting the entry, the map size is decremented.
 *
 * @param map A pointer to the hash map from which to delete the key-value pair.
 * @param key The key to delete.
 * @param key_size The size of the key data in bytes.
 */
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
            // Decrease size
            map->size--;
            return;
        }
        // Next index
        index = (index + 1) % map->capacity;
    }

    printf("[ERROR] Key not found\n");
}
