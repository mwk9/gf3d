#include "linked_list.h"

LLNode *llnode_new(void *data, size_t size)
{
	LLNode *node = NULL;

	if (size <= 0)
	{
		slog("Error: Cannot allocate space for 0 size.");
		return NULL;
	}

	node = (LLNode *)malloc(sizeof(LLNode) + size);
	if (!node)
	{
		slog("Error: Failed to allocate space for a new LLNode.");
		return NULL;
	}
	memset(node, 0, sizeof(LLNode) + size);

	node->data = data;
	node->size = size;
	node->next = NULL;

	return node;
}

LinkedList *ll_new(size_t dataSize)
{
	LinkedList *ll = NULL;

	if (dataSize <= 0)
	{
		slog("Error: Cannot allocate space for 0 size.");
		return NULL;
	}

	ll = (LinkedList *)malloc(sizeof(LinkedList));
	if (!ll)
	{
		slog("Error: Failed to allocate space for a new LinkedList.");
		return NULL;
	}
	memset(ll, 0, sizeof(LinkedList));

	ll->head = NULL;
	ll->dataSize = dataSize;

	return ll;
}

void ll_insert(LinkedList *ll, void *data, size_t size)
{
	LLNode *node = NULL;
	LLNode *curr = NULL;

	if (!ll)
	{
		slog("Error: Cannot insert into a LinkedList that is NULL. Did you pass in a valid pointer returned from ll_new?");
		return;
	}

	node = llnode_new(data, size);
	if (!node)
	{
		slog("Error: Failed to insert into the LinkedList.");
		return;
	}

	curr = ll->head;
	if (curr == NULL)
	{
		//first insert, nothing at the start of the LinkedList
		ll->head = node;
		return;
	}
	while (curr->next != NULL)
	{
		//keep iterating through the LinkedList until you find
		//the last item in the LinkedList
		curr = curr->next;
	}
	//then assign the last node to be the new node
	curr->next = node;
}

void *ll_remove_head(LinkedList *ll)
{
	LLNode *head = NULL;
	void *data = NULL;

	if (!ll)
	{
		slog("Error: Cannot remove head from a NULL LinkedList. Did you pass in a valid pointer returned from ll_new?");
		return NULL;
	}

	head = ll->head;
	data = head->data;
	ll->head = ll->head->next;
	memset(head, 0, sizeof(LLNode) + head->size);
	free(head);
	return data;
}

void ll_delete(LinkedList *ll)
{
	LLNode *curr = NULL;

	if (!ll)
	{
		slog("Error: Cannot delete from a NULL LinkedList. Did you pass in a valid pointer returned from ll_new?");
		return NULL;
	}

	while (ll->head != NULL)
	{
		curr = ll->head;
		ll->head = ll->head->next;
		memset(curr, 0, sizeof(LLNode) + curr->size);
		free(curr);
	}
}

unsigned int ll_count(LinkedList *ll)
{
	unsigned int count = 0;
	LLNode *curr = NULL;

	if (!ll)
	{
		return 0;
	}

	curr = ll->head;
	while (curr)
	{
		curr = curr->next;
		count++;
	}

	return count;
}
