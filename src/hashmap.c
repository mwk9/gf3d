#include "hashmap.h"
#include "simple_logger.h"
#include "gf3d_vector.h"

int a_hash_i_guess(int key)
{
	return 1;
}

Vector3D hash_to_space_bucket(Vector3D pos)
{
	int x, y, z;

	x = pos.x / HASH_PRECISION;
	y = pos.y / HASH_PRECISION;
	z = pos.z / HASH_PRECISION;

	return vector3d(x, y, z);
}

Hashmap *hashmap_init(unsigned int maxNodes)
{
	Hashmap *hashbrown = NULL;
	hashbrown = (Hashmap *)malloc(sizeof(Hashmap));
	if (!hashbrown)
	{
		slog("Error: Could not allocate memory for a new Hashmap.");
		return NULL;
	}
	memset(hashbrown, 0, sizeof(Hashmap));
	hashbrown->map = (HashmapNode *)malloc(sizeof(HashmapNode) * maxNodes);
	if (!hashbrown->map)
	{
		slog("Error: Could not allocate memory for the Map.");
		return NULL;
	}
	memset(hashbrown->map, 0, sizeof(HashmapNode) * maxNodes);
	hashbrown->maxNodes = maxNodes;
	return hashbrown;
}

HashmapNode *hashmap_new_node(size_t keySize, size_t elementSize)
{
	HashmapNode *node = NULL;

	if (keySize <= 0)
	{
		slog("Error: Key Size is <= 0.");
		return NULL;
	}
	if (elementSize <= 0)
	{
		slog("Error: Element Size is <= 0.");
		return NULL;
	}

	node = (HashmapNode *)malloc(sizeof(HashmapNode) + keySize);
	if (!node)
	{
		slog("Error: Could not allocate memory for a new HashmapNode");
		return NULL;
	}
	memset(node, 0, sizeof(HashmapNode) + keySize);
	/*node->data = llnode_new()
	if (!node->data)
	{
		slog("Error: Could not allocate memory for a new HashmapNode");
		free(node);
		return NULL;
	}*/
	node->keySize = keySize;
	return node;
}

Hashmap *hashmap_rehash(Hashmap *map)
{
	return NULL;
}

int hashmap_insert(Hashmap **map, void *key, size_t keySize, void *data, size_t elementSize)
{
	//unsigned long _key = a_hash_i_guess(key);
	int index = 0;
	int _index = 0;
	HashmapNode *new_node = NULL;

	if (!map)
	{
		slog("Error: Cannot insert into a NULL Hashmap");
		return -1;
	}
	new_node = hashmap_new_node(keySize, elementSize);
	if (!new_node)
	{
		return -2;
	}

	new_node->key = key;
	new_node->keySize = keySize;
	new_node->data = llnode_new(data, elementSize);
	if (!new_node->data)
	{
		slog("BLEH");
		return -1;
	}

	return 0;
}

void *hashmap_get_data(Hashmap *map, int key)
{
	return NULL;
}

void *hashmap_delete(Hashmap **map, int key)
{
	return NULL;
}

int hashmap_clear(Hashmap *map)
{
	return 0;
}

void hashmap_print(Hashmap *map)
{

}
