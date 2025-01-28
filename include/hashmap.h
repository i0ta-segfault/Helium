// https://theartincode.stanis.me/008-djb2/
#ifndef _HASHMAP_H_
#define _HASHMAP_H_

    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #define HASHMAP_LOAD_FACTOR 0.75

    typedef struct EntryMetaData {
        char* data_type;            // store the variable data type 
        void* value;                // could hold any type of value really (int, float, etc...)
    } EntryMetaData;

    typedef struct HashMapEntry {
        char* key;                  // variable name
        EntryMetaData meta_data;
        struct HashMapEntry* next;
    } HashMapEntry;

    typedef struct HashMap {
        HashMapEntry** entries;
        size_t capacity;
        size_t size;
    } HashMap;

    HashMap* initHashMap(size_t);
    size_t hashFunction(const char*, size_t);
    void insertToHashMap(HashMap*, const char*, const char*, void*);
    EntryMetaData getFromHashMap(HashMap*, const char*);
    void resizeHashMap(HashMap*);
    void freeHashMap(HashMap*);

#endif