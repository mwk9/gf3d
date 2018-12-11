#ifndef __MULTITREE__
#define __MULTITREE__

#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

#define MAX_MULTITREE_NODE_CHILDREN 16
#define DEFAULT_NEGATIVE_OFFSET 7

typedef struct multitree_node_s
{
	void *data;
	size_t dataSize;
	unsigned int maxChildren;
	unsigned int index;
	struct multitree_node_s *children[MAX_MULTITREE_NODE_CHILDREN];
	unsigned int negativeOffset;
}MultitreeNode;

MultitreeNode *multitree_node_init(unsigned int maxChildren, unsigned int negativeOffset, unsigned int index);
int multitree_insert_data_into_node(MultitreeNode *node, void *data, size_t dataSize);
int multitree_new_child(MultitreeNode *node, int index);
int multitree_insert_data_into_xyz_node(MultitreeNode *head, unsigned int x, unsigned int y, unsigned int z, void *data, size_t dataSize);

#endif //!__MULTITREE