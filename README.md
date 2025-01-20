# HashMap - by _ilBuso_
A simple implementation of a **HashMap** in **C**. This implementation supports **generic keys** and **values**, using a **custom hash function** and an **equality check** for comparisons.

## Features
- **Generic keys and values**: Supports custom data types by using void pointers and custom key size.
- **Customizable hash function**: Allows you to define your own hash function.
- **Collision resolution**: Uses **linear probing** for handling collisions.
- **Memory management**: Allocates and frees memory properly to avoid memory leaks.

## File Structure
- `hashmap.h`: The header file defining the `HashMap` structure, function prototypes, and documentation.
- `hashmap.c`: The source file containing the actual implementation of the hash map functions.
- `main.c`: An example file for testing the functionality of the hash map (optional).

## Example Usage
```c
#include "hashmap.h"
#include <stdio.h>

int main() {
    HashMap map;
    hashmap_init(&map, 10);

    int key1 = 42;
    char value1[] = "Hello, World!";
    
    insert(&map, &key1, sizeof(key1), value1);

    char* result = (char*)find(&map, &key1, sizeof(key1));
    if (result) {
        printf("Found: %s\n", result);
    } else {
        printf("Key not found.\n");
    }

    hashmap_cleanup(&map);
    return 0;
}
```

## Usage
To compile the project, thanks to the integrate `makefile`, use the following command:
```bash
make
```

## Acknowledgements
- The hash map implementation follows a simple design using **linear probing** for collision resolution.
- The **FNV-1a hash algorithm** is used for hashing keys.