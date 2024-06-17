#ifndef HASHMAP_H
#define HASHMAP_H

#define DEFAULT_MAP_SIZE 10
#define DEFAULT_RESIZE_MULTIPLE 2

typedef u_int32_t(HashFunction)(char *, u_int32_t);

enum HashMapEntryValueType
{
    INT_ARR_t,
    INT_t,
    CHAR_ARR_t,
    DYN_ARR_t,
    MAP_T,
};

typedef struct hashMapEntry
{
    char *key;
    void *value;
    enum HashMapEntryValueType value_type;
    struct hashMapEntry *next;
} HashMapEntry;

typedef struct
{
    u_int32_t size;
    u_int32_t capacity;
    HashMapEntry **entries;
    HashFunction *hashFunction;
} HashMap;

extern HashMapEntry *HashMapEntryInit(char *, void *, enum HashMapEntryValueType);

extern HashMap *HashMapInit(u_int32_t, HashFunction *);
extern HashMap *DefaultHashMapInit(void);
extern void FreeHashMap(HashMap *);

extern void HashMapInsert(HashMap *, HashMapEntry *);
extern HashMapEntry *HashMapGet(HashMap *, char *);
extern void HashMapRemove(HashMap *, char *);

extern void PrintHashMap(HashMap *);
#endif
