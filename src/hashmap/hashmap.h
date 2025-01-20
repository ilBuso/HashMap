/**
 *  @file   hashmap.h
 *  @brief  Simple implementation of a HashMap.
 *
 *  This file defines the data structures and function prototypes for a simple
 *  implementation of a hash map that supports generic keys and values. It
 *  includes functions for inserting, finding, and deleting key-value pairs in
 *  the hash map.
 *
 *  @author Alessandro Busola
 *  @date   January 2025
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Structure representing a key in the hash map.
 *
 * This structure holds a generic key and its associated size. It allows storing
 * arbitrary key types, which is important for generic hash maps.
 */
typedef struct {
    void* key;       /**< Pointer to the key data */
    size_t key_size; /**< Size of the key data */
} Key;

/**
 * @brief Structure representing a key-value pair in the hash map.
 *
 * This structure holds a key and its associated value. The value is stored as a
 * generic pointer, allowing any type of data to be stored in the hash map.
 */
typedef struct {
    Key key;     /**< The key associated with the value */
    void* value; /**< Pointer to the value data */
} HashMap_Entry;

/**
 * @brief HashMap structure that holds the data and provides functionality for
 *        inserting, searching, and deleting key-value pairs.
 *
 * The `HashMap` structure is the main container for the hash map. It contains a
 * dynamic array of `HashMap_Entry` structures and supports operations like
 * insertion, searching, and deletion based on generic keys.
 */
typedef struct HashMap {
    HashMap_Entry*
        table; /**< Pointer to the hash table storing key-value pairs */
    size_t
        capacity; /**< The total capacity of the hash table (number of slots) */
    size_t size;  /**< The current number of entries in the hash map */

    /**
     * @brief Function pointer for the hash function.
     *
     * This function is used to compute a hash value for a given key. It takes
     * the key and its size as input and returns a hash value.
     */
    size_t (*hash_function)(const void* key, size_t key_size);

    /**
     * @brief Function pointer for the equality function.
     *
     * This function is used to compare two keys for equality. It should return
     * true if the keys are equal and false otherwise.
     */
    bool (*equals)(const void* a, const void* b, size_t key_size);

    /**
     * @brief Function pointer for the insert function.
     *
     * This function is used to insert a new key-value pair into the hash map.
     */
    void (*insert)(struct HashMap* map, void* key, size_t key_size,
                   void* value);

    /**
     * @brief Function pointer for the find function.
     *
     * This function is used to find the value associated with a given key. It
     * returns a pointer to the value, or NULL if the key is not found.
     */
    void* (*find)(struct HashMap* map, void* key, size_t key_size);

    /**
     * @brief Function pointer for the delete function.
     *
     * This function is used to remove a key-value pair from the hash map.
     */
    void (*delete_entry)(struct HashMap* map, void* key, size_t key_size);

} HashMap;

/**
 * @brief Initializes a hash map with a specified capacity.
 *
 * This function initializes the hash map by allocating memory for the table and
 * setting the initial size and capacity. It should be called before using the
 * map.
 *
 * @param map The hash map to initialize.
 * @param capacity The initial capacity of the hash map (number of slots).
 */
void hashmap_init(HashMap* map, size_t capacity);

/**
 * @brief Cleans up the resources used by the hash map.
 *
 * This function frees the memory allocated for the hash map's table and its
 * entries. It should be called when the hash map is no longer needed to prevent
 * memory leaks.
 *
 * @param map The hash map to clean up.
 */
void hashmap_cleanup(HashMap* map);

/**
 * @brief Computes a hash value for a given key.
 *
 * This function computes a hash value for the key, which is used to determine
 * where to store the key-value pair in the hash map. The hash function is
 * generic, allowing it to work with various types of keys.
 *
 * @param key The key to hash.
 * @param key_size The size of the key data.
 *
 * @return A hash value for the key.
 */
size_t hash_key(const void* key, size_t key_size);

/**
 * @brief Compares two keys for equality.
 *
 * This function compares two keys to determine if they are equal. It is used by
 * the hash map to handle collisions and check if a key already exists.
 *
 * @param a The first key.
 * @param b The second key.
 * @param key_size The size of the key data.
 *
 * @return true if the keys are equal, false otherwise.
 */
bool key_equals(const void* a, const void* b, size_t key_size);

/**
 * @brief Inserts a key-value pair into the hash map.
 *
 * This function inserts a key-value pair into the hash map. If the key already
 * exists, it updates the value. It uses the hash function to find the
 * appropriate position in the table and handles collisions if necessary.
 *
 * @param map The hash map into which to insert the key-value pair.
 * @param key The key to insert.
 * @param key_size The size of the key data.
 * @param value The value to associate with the key.
 */
void insert(HashMap* map, void* key, size_t key_size, void* value);

/**
 * @brief Finds the value associated with a key in the hash map.
 *
 * This function looks up the value associated with the given key. It uses the
 * hash function to locate the correct position in the table. If the key is
 * found, it returns a pointer to the associated value, otherwise NULL.
 *
 * @param map The hash map to search in.
 * @param key The key to search for.
 * @param key_size The size of the key data.
 *
 * @return A pointer to the value associated with the key, or NULL if not found.
 */
void* find(HashMap* map, void* key, size_t key_size);

/**
 * @brief Deletes a key-value pair from the hash map.
 *
 * This function removes the key-value pair associated with the given key from
 * the hash map. If the key is not found, the hash map remains unchanged.
 *
 * @param map The hash map from which to delete the key-value pair.
 * @param key The key to delete.
 * @param key_size The size of the key data.
 */
void delete_entry(HashMap* map, void* key, size_t key_size);

#endif