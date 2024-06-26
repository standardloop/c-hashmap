#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./hashmap.h"

static void freeHashMapEntryValue(void *);
static void freeHashMapEntrySingle(HashMapEntry *, bool);
static void freeHashMapEntryList(HashMapEntry *, bool);
static void freeHashMapEntries(HashMapEntry **, u_int32_t, bool, bool);
static void printHashMapEntry(HashMapEntry *, u_int32_t);
static u_int32_t defaultHashFunction(char *, u_int32_t);

static bool hashMapEntriesInsert(HashMapEntry **, u_int32_t, HashMapEntry *);

static HashMapEntry **hashMapEntriesInit(u_int32_t);

static inline bool isMapFull(HashMap *);
static void hashMapResize(HashMap *map);

// Jenkins's one_at_a_time
static u_int32_t defaultHashFunction(char *key, u_int32_t capacity)
{
    u_int32_t len = strlen(key);
    uint32_t hash = 0;

    for (uint32_t i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % capacity;
}

extern HashMap *DefaultHashMapInit(void)
{
    return HashMapInit(DEFAULT_MAP_SIZE, NULL);
}

static HashMapEntry **hashMapEntriesInit(u_int32_t capacity)
{
    HashMapEntry **entries = malloc(sizeof(HashMapEntry *) * capacity);
    if (entries == NULL)
    {
        return NULL;
    }

    for (u_int32_t i = 0; i < capacity; i++)
    {
        entries[i] = NULL;
    }

    return entries;
}

// FIXME add resize as well
extern HashMap *HashMapInit(u_int32_t initial_capacity, HashFunction *hashFunction)
{
    HashMap *map = malloc(sizeof(HashMap));
    if (map == NULL)
    {
        printf("[ERROR]: could not allocate memory for HashMap\n");
        return NULL;
    }
    map->size = 0;
    map->capacity = initial_capacity;
    map->entries = hashMapEntriesInit(initial_capacity);

    if (map->entries == NULL)
    {
        FreeHashMap(map);
        return NULL;
    }

    if (hashFunction == NULL)
    {
        map->hashFunction = defaultHashFunction;
    }
    return map;
}

static inline bool isMapFull(HashMap *map)
{
    return map->capacity == map->size;
}

extern void HashMapInsert(HashMap *map, HashMapEntry *entry)
{
    if (map == NULL || entry->key == NULL || entry->value == NULL)
    {
        return;
    }
    if (isMapFull(map))
    {
        hashMapResize(map);
    }
    u_int32_t index = map->hashFunction(entry->key, map->capacity);
    bool collision = hashMapEntriesInsert(map->entries, index, entry);
    if (!collision)
    {
        map->size++;
    }
}

static bool hashMapEntriesInsert(HashMapEntry **entries, u_int32_t index, HashMapEntry *entry)
{
    HashMapEntry *collision = entries[index];
    if (collision == NULL)
    {
        entries[index] = entry;
        return false;
    }

    HashMapEntry *iterator_prev = collision;
    HashMapEntry *iterator = collision->next;
    while (iterator != NULL)
    {
        iterator = iterator->next;
        iterator_prev = iterator_prev->next;
    }
    iterator_prev->next = entry;
    return true;
}

extern HashMapEntry *HashMapGet(HashMap *map, char *key)
{
    if (map == NULL || key == NULL)
    {
        return NULL;
    }
    u_int32_t index = map->hashFunction(key, map->capacity);
    HashMapEntry *entry = map->entries[index];
    if (entry == NULL)
    {
        return NULL;
    }

    HashMapEntry *iterator = entry;
    while (iterator != NULL)
    {
        if (strcmp(key, iterator->key) == 0)
        {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;
}

static void freeHashMapEntryValue(void *value)
{
    // WIP, pass value type as well
    if (value != NULL)
    {
        free(value);
    }
}

static void freeHashMapEntryList(HashMapEntry *entry, bool deep)
{
    HashMapEntry *temp = NULL;
    while (entry != NULL)
    {
        temp = entry;
        entry = entry->next;
        if (deep)
        {
            freeHashMapEntryValue(temp->value);
        }

        free(temp);
    }
}

static void freeHashMapEntrySingle(HashMapEntry *entry, bool deep)
{
    if (entry != NULL)
    {
        if (deep)
        {
            freeHashMapEntryValue(entry->value);
        }

        free(entry);
    }
}

static void freeHashMapEntries(HashMapEntry **entries, u_int32_t size, bool deep, bool entry_values)
{
    if (entries == NULL)
    {
        return;
    }
    if (deep)
    {
        for (u_int32_t i = 0; i < size; i++)
        {
            freeHashMapEntryList(entries[i], entry_values);
        }
    }
    free(entries);
}

extern void FreeHashMap(HashMap *map)
{
    if (map == NULL)
    {
        return;
    }
    if (map->entries != NULL)
    {
        freeHashMapEntries(map->entries, map->capacity, true, true);
    }
    free(map);
}

extern HashMapEntry *HashMapEntryInit(char *key, void *value, enum HashMapEntryValueType value_type)
{
    if (key == NULL || value == NULL)
    {
        return NULL;
    }

    HashMapEntry *entry = malloc(sizeof(HashMapEntry));
    if (entry == NULL)
    {
        return NULL;
    }
    entry->key = key;
    entry->value = value;
    entry->value_type = value_type;
    entry->next = NULL;

    return entry;
}

extern void HashMapRemove(HashMap *map, char *key)
{
    if (map == NULL)
    {
        return;
    }
    u_int32_t index = map->hashFunction(key, map->capacity);
    HashMapEntry *entry = map->entries[index];
    if (entry == NULL)
    {
        return;
    }
    if (entry->next == NULL)
    {
        freeHashMapEntrySingle(entry, true);
        map->entries[index] = NULL;
        map->size--;
        return;
    }

    if (strcmp(key, entry->key) == 0)
    {
        map->entries[index] = entry->next;
        freeHashMapEntrySingle(entry, true);
        // map->size--;
        return;
    }

    HashMapEntry *iterator_prev = entry;
    HashMapEntry *iterator = entry->next;
    while (iterator != NULL)
    {
        if (strcmp(key, iterator->key) == 0)
        {
            iterator_prev->next = iterator->next;
            map->entries[index] = iterator_prev;
            freeHashMapEntrySingle(iterator, true);
            // map->size--;
            break;
        }
        iterator_prev = iterator_prev->next;
        iterator = iterator->next;
    }
}

extern void PrintHashMap(HashMap *map)
{
    if (map == NULL)
    {
        return;
    }
    if (map->size == 0)
    {
        printf("Map is currently Empty!\n");
    }
    printf("[\n");
    for (u_int32_t i = 0; i < map->capacity; i++)
    {
        HashMapEntry *entry = map->entries[i];
        if (entry != NULL)
        {
            printf("  ");
            printHashMapEntry(entry, i);
            if (i != map->capacity - 1)
            {
                printf(",");
            }
            printf("\n");
        }
    }
    printf("]\n");
}

static void printHashMapEntry(HashMapEntry *entry, u_int32_t index)
{
    // FIXME, check for value type

    HashMapEntry *iterator = entry;
    printf("%u: {", index);
    while (iterator != NULL)
    {

        printf("%s: %s", iterator->key, (char *)iterator->value);

        iterator = iterator->next;
        if (iterator != NULL)
        {
            printf(", ");
        }
    }
    printf("}");
}

static void hashMapResize(HashMap *map)
{
    if (map == NULL)
    {
        return;
    }
    printf("Resizing Hash Map!\n");

    u_int32_t new_capacity = map->capacity * DEFAULT_RESIZE_MULTIPLE;
    u_int32_t new_size = 0;

    HashMapEntry **new_entries = hashMapEntriesInit(new_capacity);

    if (new_entries == NULL)
    {
        return;
    }

    for (u_int32_t i = 0; i < map->capacity; i++)
    {
        HashMapEntry *entry = map->entries[i];
        HashMapEntry *iterator = entry;

        while (iterator != NULL)
        {
            HashMapEntry *new_entry = HashMapEntryInit(iterator->key, iterator->value, iterator->value_type); // need to move entry and not have to worry about next field
            u_int32_t new_index = map->hashFunction(iterator->key, new_capacity);
            bool collision = hashMapEntriesInsert(new_entries, new_index, new_entry);
            if (!collision)
            {
                new_size++;
            }
            iterator = iterator->next;
        }
    }
    freeHashMapEntries(map->entries, map->capacity, true, false);

    map->size = new_size;
    map->capacity = new_capacity;
    map->entries = new_entries;
}
