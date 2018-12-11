#ifndef __HASHMAP__
#define __HASHMAP__

#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

#define HASH_PRECISION 100

typedef struct hashmapnode_s
{
	void *key;
	size_t keySize;
	LLNode *data;
}HashmapNode;

typedef struct hashmap_s
{
	HashmapNode *map;
	unsigned int maxNodes;
}Hashmap;

Hashmap *hashmap_init(unsigned int maxNodes);
HashmapNode *hashmap_new_node(size_t keySize, size_t elementSize);
Hashmap *hashmap_rehash(Hashmap *map);
int hashmap_insert(Hashmap **map, void *key, size_t keySize, void *data, size_t elementSize);
void *hashmap_get_data(Hashmap *map, int key);
void *hashmap_delete(Hashmap **map, int key);
int hashmap_clear(Hashmap *map);
void hashmap_print(Hashmap *map);

#endif //!__HASHMAP__