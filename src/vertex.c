#include "vertex.h"

//Vert *vertices;
//VkBuffer vertexBuffer;
VkMemoryRequirements memRequirements = { 0 };
VkBufferCreateInfo bufferInfo = { 0 };
VkPhysicalDeviceMemoryProperties memProperites = { 0 };
VkMemoryAllocateInfo allocInfo = { 0 };
VkDevice vertexBufferMemory = { 0 };

void vertices_test()
{
	//maybe change position to vector3d
	vertices = (Vert *)malloc(sizeof(Vert) * 3);
	memset(vertices, 0, sizeof(Vert) * 3);
	vertices[0].position = vector2d(0.0, -0.5);
	vertices[0].color = vector3d(1.0, 0.0, 0.0);
	vertices[1].position = vector2d(0.5, 0.5);
	vertices[1].color = vector3d(0.0, 1.0, 0.0);
	vertices[2].position = vector2d(-0.5, 0.5);
	vertices[2].color = vector3d(0.0, 0.0, 1.0);
}

void setup_memory_allocate_info()
{
	//allocInfo = { 0 };
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = find_memory_type(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	if (vkAllocateMemory(gf3d_vgraphics_select_device(), &allocInfo, NULL, &vertexBufferMemory) != VK_SUCCESS)
	{
		slog("Error: Failed to allocate vertex buffer memory");
		return;
	}
	vkBindBufferMemory(gf3d_vgraphics_select_device(), vertexBuffer, vertexBufferMemory, 0);
}

void fill_vertex_buffer(VkDevice device)
{
	void *data;
	vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices, (size_t)bufferInfo.size);
	vkUnmapMemory(device, vertexBufferMemory);

	
}

void vertex_system_init()
{
	VkDevice device = gf3d_vgraphics_select_device();

	vertices_test();
	create_vertex_buffer(device);
	setup_memory_allocate_info();
	fill_vertex_buffer(device);
}

Vert * vert_new(Vector2D position, Vector3D color)
{
	Vert *v = NULL;
	v = (Vert *)malloc(sizeof(Vert));
	v->position = position;
	v->color = color;
	return v;
}

VkVertexInputBindingDescription getBindingDescription()
{
	VkVertexInputBindingDescription desc = {0};
	desc.binding = 0;
	desc.stride = sizeof(Vert);
	desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return desc;
}

VkVertexInputAttributeDescription * getAttributeDescriptions(Vert *vertex)
{
	VkVertexInputAttributeDescription * desc = NULL;
	desc = (VkVertexInputAttributeDescription *)malloc(sizeof(VkVertexInputAttributeDescription) * 2);
	desc[0].binding = 0;
	desc[0].location = 0;
	desc[0].format = VK_FORMAT_R32G32_SFLOAT;
	desc[0].offset = offsetof(Vert, position);
	desc[1].binding = 0;
	desc[1].location = 1;
	desc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	desc[1].offset = offsetof(Vert, color);
	return desc;
}

uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	//get device from other func vvv maybe?
	//VkPhysicalDeviceMemoryProperties memProperites;
	int i = 0;
	vkGetPhysicalDeviceMemoryProperties(gf3d_vgraphics_select_device(), &memProperites);

	for (i = 0; i < memProperites.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperites.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	slog("Error: Failed to find suitable memory type!");
	return UINT_MAX;
}

void create_vertex_buffer(VkDevice device)
{
	//VkBufferCreateInfo bufferInfo = { 0 };

	if (device == NULL)
	{
		slog("Error: device was null");
		return;
	}

	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * 3; //change to dynamically find number of verts
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.pQueueFamilyIndices = NULL;
	if (vkCreateBuffer(device, &bufferInfo, NULL, &vertexBuffer) != VK_SUCCESS)
	{
		slog("Error: Failed to create vertex buffer");
	}
	vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);
}

void destroy_vertex_buffer(VkDevice device)
{
	if (device == NULL)
	{
		slog("Error: device was null");
		return;
	}

	vkDestroyBuffer(device, vertexBuffer, NULL);
	vkFreeMemory(device, vertexBufferMemory, NULL);
}
