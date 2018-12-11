#include "multitree.h"
#include "simple_logger.h"

MultitreeNode *multitree_node_init(unsigned int maxChildren, unsigned int negativeOffset, unsigned int index)
{
	MultitreeNode *node = NULL;

	if (maxChildren <= 0)
	{
		slog("Warning: 0 or less children specified, defaulting to default value of (%i)", MAX_MULTITREE_NODE_CHILDREN);
		maxChildren = MAX_MULTITREE_NODE_CHILDREN;
	}

	node = (MultitreeNode *)malloc(sizeof(MultitreeNode));
	if (!node)
	{
		slog("Error: Could not allocate space for a new Multitree Node.");
		return NULL;
	}
	memset(node, 0, sizeof(MultitreeNode));
	/*node->children = (MultitreeNode *)malloc(sizeof(MultitreeNode) * maxChildren);
	if (!node->children)
	{
		slog("Error: Could not allocate space for the Multitree Node's children.");
		free(node);
		return NULL;
	}
	memset(node->children, 0, sizeof(MultitreeNode)* maxChildren);*/
	node->maxChildren = maxChildren;
	node->negativeOffset = negativeOffset;
	if (index < 0) index = 0;
	if (index >= maxChildren) index = maxChildren - 1;
	node->index = index;
	return node;
}

int multitree_insert_data_into_node(MultitreeNode *node, void *data, size_t dataSize)
{
	if (!node)
	{
		slog("Error: Cannot insert into a NULL Multitree Node.");
		return -1;
	}

	node->data = (void *)malloc(dataSize);
	if (!node->data)
	{
		slog("Error: Could not allocate memory for the data.");
		return -2;
	}
	memset(node->data, 0, dataSize);
	node->data = data;
	node->dataSize = dataSize;
	return 0;
}

int multitree_new_child(MultitreeNode *node, int index)
{
	int _index = 0;

	if (!node)
	{
		slog("Error: Cannot insert a child node into a NULL Multitree Node.");
		return -1;
	}

	_index = node->negativeOffset + index;
	if (_index >= node->maxChildren || _index < 0)
	{
		slog("Error: Index (%i) is out of range for children...max index is (%i)", _index, node->maxChildren - 1);
		return -2;
	}
	node->children[_index] = multitree_node_init(node->maxChildren, node->negativeOffset, _index);
	if (!node->children[_index])
	{
		slog("Error: what the f?");
		return -3;
	}
	return 0;
}

int multitree_delete_node(MultitreeNode *node)
{
	if (!node)
	{
		slog("Error: Cannot delete a NULL Multitree Node.");
		return -1;
	}
	if (!node->children)
	{
		slog("Error: Node's children was NULL...ummm that's wierd");
		return -2;
	}

	memset(node->children, 0, sizeof(MultitreeNode) * node->maxChildren);
	memset(node, 0, sizeof(MultitreeNode));
	free(node);
}

int multitree_insert_data_into_xyz_node(MultitreeNode *head, unsigned int x, unsigned int y, unsigned int z, void *data, size_t dataSize)
{
	MultitreeNode *curr = NULL;

	if (!head)
	{
		slog("Error: cannot insert into a null Multitree Node.");
		return -1;
	}
	if (dataSize <= 0)
	{
		slog("Error: Cannot insert data with size <= 0 (%i)", dataSize);
		return -5;
	}
	if (!head->children[x])
	{
		slog("Error: X child was NULL");
		return -2;
	}
	if (!head->children[x]->children[y])
	{
		slog("Error: Y child was NULL");
		return -3;
	}
	if (!head->children[x]->children[y]->children[z])
	{
		slog("Error: Z child was NULL");
		return -4;
	}
	curr = head->children[x]->children[y]->children[z];

	if (!curr->data)
	{
		//new linked list please!
		curr->data = ll_new(dataSize);
		ll_insert(curr->data, data, dataSize);
	}
	else
	{
		//linked list already exists, append to it
		ll_insert(curr->data, data, dataSize);
	}
}

/*int multitree_delete_bottom_node(MultitreeNode *map)
{
	int i = 0;
	int x, y, z;
	MultitreeNode *curr = NULL;

	if (!map)
	{
		slog("Error: Multitree Node was NULL.");
		return -1;
	}

	curr = map;

	for (i = 0; i < curr->maxChildren; i++)
	{
		if (curr)
	}
}*/
