#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./hashmap.h"

static void freeHashMapEntryValue(void *);
static void freeHashMapEntry(HashMapEntry *);
static void freeHashMapEntries(HashMapEntry **, u_int32_t, bool);

static u_int32_t defaultHashFunction(char *, u_int32_t);

static inline bool isMapFull(HashMap *);

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
    map->entries = malloc(sizeof(HashMapEntry *) * map->capacity);
    for (u_int32_t i = 0; i < initial_capacity; i++)
    {
        map->entries[i] = NULL;
    }
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
        // resize?
    }
    u_int32_t index = map->hashFunction(entry->key, map->capacity);
    if (map->entries[index] == NULL)
    {
        map->entries[index] = entry;
    }
    else
    {
        // linked list
    }
    map->size++;

    return;
}

extern HashMapEntry *HashMapGet(HashMap *map, char *key)
{
    if (map == NULL || key == NULL)
    {
        return NULL;
    }
    u_int32_t index = map->hashFunction(key, map->capacity);
    if (map->entries[index]->next == NULL)
    {
        return map->entries[index];
    }
    else
    {
    }
    return NULL;
}

static void freeHashMapEntryValue(void *value)
{
    if (value != NULL)
    {
        free(value);
    }
}

static void freeHashMapEntry(HashMapEntry *entry)
{
    HashMapEntry *temp = NULL;
    while (entry != NULL)
    {
        temp = entry;
        entry = entry->next;
        freeHashMapEntryValue(temp->value);
        free(temp);
    }
}

static void freeHashMapEntries(HashMapEntry **entries, u_int32_t size, bool deep)
{
    if (entries == NULL)
    {
        return;
    }
    if (deep)
    {
        for (u_int32_t i = 0; i < size; i++)
        {
            freeHashMapEntry(entries[i]);
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
        freeHashMapEntries(map->entries, map->capacity, true);
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