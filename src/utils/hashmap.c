#include "hashmap.h"

HashMap* initHashMap(size_t capacity) {
    HashMap* map = malloc(sizeof(HashMap));
    map->entries = calloc(capacity, sizeof(HashMapEntry*));
    map->capacity = capacity;
    map->size = 0;
    return map;
}

size_t hashFunction(const char* key, size_t capacity) {
    // djb2 hash fn
    size_t hash = 5381;
    while (*key) {
        hash = ((hash << 5) + hash) + *key++;
    }
    return hash % capacity;
}

void resizeHashMap(HashMap* map){
    size_t new_capacity = map->capacity * 2;
    HashMapEntry** new_entries = calloc(new_capacity, sizeof(HashMapEntry*));

    for(size_t i = 0; i < map->capacity; i++){
        HashMapEntry* entry = map->entries[i];
        while(entry){
            HashMapEntry* next = entry->next;
            size_t new_index = hashFunction(entry->key, new_capacity);
            entry->next = new_entries[new_index];
            new_entries[new_index] = entry;
            entry = next;
        }
    }

    free(map->entries);
    map->entries = new_entries;
    map->capacity = new_capacity;
}

void insertToHashMap(HashMap* map, const char* key, const char* data_type, void* value) {
    if((double)map->size / map->capacity > HASHMAP_LOAD_FACTOR)
        resizeHashMap(map);

    size_t index = hashFunction(key, map->capacity);
    HashMapEntry* entry = map->entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) { // key already exists in the map so just return
            free(entry->meta_data.data_type);
            entry->meta_data.data_type = strdup(data_type);
            entry->meta_data.value = value;
            return;
        }
        entry = entry->next;
    }
    HashMapEntry* newEntry = malloc(sizeof(HashMapEntry));
    newEntry->key = strdup(key);
    newEntry->meta_data.data_type = strdup(data_type);
    newEntry->meta_data.value = value;
    newEntry->next = map->entries[index];
    map->entries[index] = newEntry;
    map->size++;
}

EntryMetaData getFromHashMap(HashMap* map, const char* key) {
    size_t index = hashFunction(key, map->capacity);
    HashMapEntry* entry = map->entries[index];
    EntryMetaData returnmetadata = {
        .data_type = 0,
        .value = NULL
    };
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            returnmetadata.data_type = entry->meta_data.data_type;
            returnmetadata.value = entry->meta_data.value;
            return returnmetadata;
        }
        entry = entry->next;
    }
    return returnmetadata;
}

void freeHashMap(HashMap* map) {
    for (size_t i = 0; i < map->capacity; i++) {
        HashMapEntry* entry = map->entries[i];
        while (entry) {
            HashMapEntry* temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp->meta_data.data_type);
            if (temp->meta_data.value != NULL) {
                free(temp->meta_data.value);
            }
            free(temp);
        }
    }
    free(map->entries);
    free(map);
}