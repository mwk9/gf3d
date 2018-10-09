#ifndef __GF3D_VGRAPHICS_H__
#define __GF3D_VGRAPHICS_H__

#include <vulkan/vulkan.h>

#include "gf3d_vector.h"
#include "vertex.h"

#define GF3D_VGRAPHICS_DISCRETE 0   //Choosing whether to use discrete [1] or integrated graphics [0]

void gf3d_vgraphics_init(
    char *windowName,
    int renderWidth,
    int renderHeight,
    Vector4D bgcolor,
    Bool fullscreen,
    Bool enableValidation
);

VkDevice gf3d_vgraphics_get_default_logical_device();
VkExtent2D gf3d_vgraphics_get_view_extent();

void gf3d_vgraphics_clear();

void gf3d_vgraphics_render();

uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags prop);
int create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * buffer, VkDeviceMemory * bufferMemory);
VkPhysicalDevice gf3d_vgraphics_get_default_physical_device();


#endif
