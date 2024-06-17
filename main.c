#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./hashmap.h"

#define SIZE 20

static char *quickAllocatedString(char *);

static char *quickAllocatedString(char *copy)
{
    size_t true_len = strlen(copy) + 1;
    char *allocated_string = malloc(sizeof(char) * true_len);
    strcpy(allocated_string, copy);
    return allocated_string;
}

int main(void)
{

    HashMap *map = DefaultHashMapInit();
    if (map == NULL)
    {
        return EXIT_FAILURE;
    }
    HashMapEntry *example_entry;
    example_entry = HashMapEntryInit("key", quickAllocatedString("hi!"), CHAR_ARR_t);
    HashMapInsert(map, example_entry);

    example_entry = HashMapEntryInit("key2", quickAllocatedString("foo"), CHAR_ARR_t);
    HashMapInsert(map, example_entry);

    example_entry = HashMapEntryInit("name", quickAllocatedString("josh"), CHAR_ARR_t);
    HashMapInsert(map, example_entry);

    HashMapRemove(map, "key");
    HashMapEntry *deleted_entry = HashMapGet(map, "key");
    if (deleted_entry != NULL)
    {
        printf("%s\n", (char *)deleted_entry->value);
    }

    // HashMapRemove(map, "key2");
    // HashMapRemove(map, "name");

    PrintHashMap(map);

    FreeHashMap(map);

    return EXIT_SUCCESS;
}
