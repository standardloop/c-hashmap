#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./hashmap.h"

#define SIZE 20

int main(void)
{

    HashMap *map = DefaultHashMapInit();
    if (map == NULL)
    {
        return EXIT_FAILURE;
    }
    // printf("%u\n", map->hashFunction("hello", map->capacity));

    char *example_value = malloc(sizeof(char) * 4);
    strcpy(example_value, "hi!");
    HashMapEntry *example_entry = HashMapEntryInit("key", example_value, CHAR_ARR_t);
    HashMapInsert(map, example_entry);

    HashMapEntry *get_entry = HashMapGet(map, "key");
    printf("%s\n", (char *)get_entry->value);

    FreeHashMap(map);

    return EXIT_SUCCESS;
}
