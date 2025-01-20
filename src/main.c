#include "./hashmap/hashmap.h"
#include <stdio.h>

int main() {
    HashMap map;
    hashmap_init(&map, 10); 

    int key1 = 42;
    int key2 = 99;
    int value1 = 10;
    int value2 = 20;

    // Insert key-value pairs
    map.insert(&map, &key1, sizeof(key1), &value1);
    map.insert(&map, &key2, sizeof(key2), &value2);

    // Find values
    int* found_value1 = (int*)map.find(&map, &key1, sizeof(key1));
    int* found_value2 = (int*)map.find(&map, &key2, sizeof(key2));

    printf("Found value for key1: %d\n", *found_value1);
    printf("Found value for key2: %d\n", *found_value2);

    // Delete a key-value pair
    map.delete_entry(&map, &key1, sizeof(key1));

    // Try to find the deleted key
    found_value1 = (int*)map.find(&map, &key1, sizeof(key1));
    if (found_value1 == NULL) {
        printf("Key1 not found after deletion\n");
    }

    hashmap_cleanup(&map);
    return 0;
}
