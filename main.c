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

    char *example_value_2 = malloc(sizeof(char) * 4);
    strcpy(example_value_2, "foo");
    HashMapEntry *example_entry_2 = HashMapEntryInit("key2", example_value_2, CHAR_ARR_t);
    HashMapInsert(map, example_entry_2);

    HashMapEntry *get_test = HashMapGet(map, "key2");
    printf("%s\n", (char *)get_test->value);

    FreeHashMap(map);

    return EXIT_SUCCESS;
}
