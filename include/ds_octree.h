#ifndef __DS_OCTREE__
#define __DS_OCTREE__

#include <stddef.h>
#include <stdint.h>

typedef struct octree_node_s
{
	float minx, maxx;
	float miny, maxy;
	float minz, maxz;

	struct octree_node_s *parent;
	struct octree_node_s *subnodes[8];

	uint32_t numChildren;
}OctreeNode;

typedef struct octree_s
{
	OctreeNode *root;
	size_t count;
	size_t height;
}Octree;

Octree *octree_build();
void octree_free(Octree *tree);

#endif //!__DS_OCTREE__