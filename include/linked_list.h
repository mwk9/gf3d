#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_logger.h"

typedef struct ll_node_s
{
	void *data;
	size_t size;
	struct ll_node_s *next;
}LLNode;

typedef struct ll_s
{
	LLNode *head;
	size_t dataSize;
}LinkedList;

/**
 * @brief Allocates memory for a new LLNode
 * @param data A void pointer to the data to hold
 * @param size The size of the data the void pointer holds
 * @returns A pointer to a new LLNode if successful; NULL if failed
 */
LLNode *llnode_new(void *data, size_t size);

/**
 * @brief Creates a new LinkedList
 * @param dataSize The size of the data the nodes will hold
 * @returns A pointer to a new LinkedList if successful; NULL if failed
 */
LinkedList *ll_new(size_t dataSize);

/**
 * @brief Inserts a new node into a LinkedList
 * @param ll The LinkedList to insert into
 * @param data Void pointer to the data to hold
 * @param size The size of the data held by the void pointer
 */
void ll_insert(LinkedList *ll, void *data, size_t size);

/**
 * @brief Removes the head node of the LinkedList and returns its data
 * @param ll The LinkedList to remove the head node from
 * @returns Void pointer to the data of the head node if successful; NULL if failed
 */
void *ll_remove_head(LinkedList *ll);

/**
 * @brief Deletes a LinkedList, freeing its memory
 * @param ll The LinkedList to delete
 */
void ll_delete(LinkedList *ll);

#endif //!__LINKED_LIST__