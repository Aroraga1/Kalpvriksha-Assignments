#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 2003   

struct Node {
    int key;
    char value[100];
    struct Node *prev;
    struct Node *next;
};

struct HashEntry {
    int key;
    Node *node;
    struct HashEntry *next;
};

struct LRUCache{
    int capacity;
    int size;
    Node *head;   
    Node *tail;  
    HashEntry *map[HASH_SIZE];
};

int hash(int key) {
    return key % HASH_SIZE;
}

Node* hashGet(LRUCache *cache, int key) {
    int index = hash(key);
    HashEntry *entry = cache->map[index];

    while (entry) {
        if (entry->key == key)
            return entry->node;
        entry = entry->next;
    }
    return NULL;
}

void hashPut(LRUCache *cache, int key, Node *node) {
    int index = hash(key);
    HashEntry *entry = (HashEntry *)malloc(sizeof(HashEntry));

    entry->key = key;
    entry->node = node;
    entry->next = cache->map[index];
    cache->map[index] = entry;
}

void hashRemove(LRUCache *cache, int key) {
    int index = hash(key);
    HashEntry *curr = cache->map[index];
    HashEntry *prev = NULL;

    while (curr) {
        if (curr->key == key) {
            if (prev)
                prev->next = curr->next;
            else
                cache->map[index] = curr->next;

            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void moveAtFront(LRUCache *cache, Node *node) {
    if (node == cache->head)
        return;

    if (node->prev)
        node->prev->next = node->next;

    if (node->next)
        node->next->prev = node->prev;

    if (node == cache->tail)
        cache->tail = node->prev;

    node->prev = NULL;
    node->next = cache->head;

    if (cache->head)
        cache->head->prev = node;

    cache->head = node;

    if (cache->tail == NULL)
        cache->tail = node;
}

void addAtFront(LRUCache *cache, Node *node) {
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head)
        cache->head->prev = node;

    cache->head = node;

    if (cache->tail == NULL)
        cache->tail = node;
}

void removeTail(LRUCache *cache) {
    if (cache->tail == NULL)
        return;

    Node *temp = cache->tail;
    hashRemove(cache, temp->key);

    if (temp->prev)
        temp->prev->next = NULL;

    cache->tail = temp->prev;

    if (cache->tail == NULL)
        cache->head = NULL;

    free(temp);
    cache->size--;
}

LRUCache* createCache(int capacity) {
    LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for (int i = 0; i < HASH_SIZE; i++)
        cache->map[i] = NULL;

    return cache;
}

char* get(LRUCache *cache, int key) {
    Node *node = hashGet(cache, key);

    if (!node)
        return NULL;

    moveAtFront(cache, node);
    return node->value;
}

void put(LRUCache *cache, int key, char *value) {
    Node *node = hashGet(cache, key);

    if (node) {
        strcpy(node->value, value);
        moveAtFront(cache, node);
        return;
    }

    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->prev = newNode->next = NULL;

    addAtFront(cache, newNode);
    hashPut(cache, key, newNode);
    cache->size++;

    if (cache->size > cache->capacity)
        removeTail(cache);
}

int main() {
    LRUCache *cache = NULL;
        char command[20];

    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "createCache") == 0) {
            int cap;
        scanf("%d", &cap);
            cache = createCache(cap);
        }
        else if (strcmp(command, "put") == 0) {
            int key;
            char value[100];
            scanf("%d %s", &key, value);
            put(cache, key, value);
        }
        else if (strcmp(command, "get") == 0) {
            int key;
            scanf("%d", &key);
            char *result = get(cache, key);
        if (result)
                printf("%s\n", result);
            else
                printf("NULL\n");
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    return 0;
}
