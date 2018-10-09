#ifndef __VERTEX__
#define __VERTEX__

#include <vulkan/vulkan.h>
//#include <stdio.h>
//#include <iostream>
#include <string.h>
#include "gf3d_vector.h"
#include "simple_logger.h"

typedef struct vertex_s
{
	Vector2D position;
	Vector3D color;
}Vert;

Vert *vertices;
VkBuffer vertexBuffer;
//VkMemoryRequirements memRequirements;
//VkBufferCreateInfo bufferInfo;
//VkPhysicalDeviceMemoryProperties memProperites;
//VkMemoryAllocateInfo allocInfo;
//VkDevice vertexBufferMemory;

void vertex_system_init();
Vert * vert_new(Vector2D position, Vector3D color);
VkVertexInputBindingDescription getBindingDescription();
VkVertexInputAttributeDescription * getAttributeDescriptions(Vert *vertex);
uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);
void create_vertex_buffer(VkDevice *device);
void destroy_vertex_buffer(VkDevice *device);
VkPhysicalDevice gf3d_vgraphics_select_device();

#endif //!__VERTEX__
