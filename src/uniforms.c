#include <gf3d_vgraphics.h>
#include "uniforms.h"

/**
 * @brief Determines the size of a UBO on the GPU
 * @returns The size of a UBO on the GPU card
 */
Uint32 uniforms_aligned_size()
{
	static VkPhysicalDevice foundDevice = VK_NULL_HANDLE;
	static Uint32 alignedUniformSize = 0;
	VkPhysicalDevice currentDevice = gf3d_vgraphics_get_default_physical_device();
	VkPhysicalDeviceProperties properties;
	Uint32 minAlignment = 0;

	if (currentDevice != foundDevice)
	{
		foundDevice = currentDevice;
		vkGetPhysicalDeviceProperties(currentDevice, &properties);
		minAlignment = (Uint32)properties.limits.minUniformBufferOffsetAlignment;
		alignedUniformSize = uniforms_size();

		//Check to see if the UBO needs to be padded with extra data, so it
		//is formatted correctly in memory (proper size of an entry in an array)
		if (alignedUniformSize % minAlignment > 0)
		{
			alignedUniformSize += minAlignment - (alignedUniformSize % minAlignment);
		}
	}

	return alignedUniformSize;
}

/**
 * @brief Allocates UBO memory on the GPU for the UBOManager
 * @param self The pointer to the UBOManager to allocate memory for
 */
void uniforms_allocate_buffer(UBOManager *self)
{
	self->uboBuffersSize = self->numUBOs * uniforms_aligned_size();
	gf3d_vgraphics_create_buffer
	(
		self->uboBuffersSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		&self->uboBuffer,
		&self->uboBuffersDeviceMemory
	);
}

void uniforms_init_buffer_objects(UBOManager *self, float aspectRatio)
{
	static UniformBufferObject localUBO;
	Uint32 i = 0;

	gf3d_matrix_identity(localUBO.model);
	gf3d_matrix_identity(localUBO.view);
	gf3d_matrix_identity(localUBO.proj);
	gf3d_matrix_view
	(
		localUBO.view,
		vector3d(2.0f, 20.0f, 2.0f),
		vector3d(0.0f, 0.0f, 0.0f),
		vector3d(0.0f, 0.0f, 1.0f)
	);
	gf3d_matrix_perspective(localUBO.proj, (float)(45 * GF3D_DEGTORAD), aspectRatio, 0.1f, 100.0f);
	localUBO.proj[1][1] *= -1;
	for (i = 0; i < self->numEntities; i++)
	{
		UniformBufferObject *const ubo = uniforms_get_local_reference(self, i);
		memcpy(ubo, &localUBO, uniforms_size());
	}
}

UBOManager * ubo_manager_init(Uint32 numEntities, Uint32 numSwapchainImages, int renderWidth, int renderHeight)
{
	UBOManager *manager = NULL;
	Uint32 alignedSize = 0;

	manager = (UBOManager *)malloc(sizeof(UBOManager));
	memset(manager, 0, sizeof(UBOManager));

	manager->numEntities = numEntities;
	manager->numSwapchainImages = numSwapchainImages;
	manager->numUBOs = numEntities * numSwapchainImages;
	alignedSize = uniforms_aligned_size();
	slog("sizeof UBO is (%i), aligned size is (%i)", sizeof(UniformBufferObject), alignedSize);
	manager->currentUBOStates = (UniformBufferObject *)malloc(alignedSize * manager->numEntities);
	memset(manager->currentUBOStates, 0, alignedSize * manager->numEntities);
	//manager->mappedUBOBuffers = (UniformBufferObject *)malloc(alignedSize * numEntities * sizeof(Entity));
	//memset(manager->mappedUBOBuffers, 0, alignedSize * numEntities * sizeof(Entity));

	//buffer allocate
	uniforms_allocate_buffer(manager);
	//init buffer objects
	uniforms_init_buffer_objects(manager, ((float)renderWidth) / ((float)renderHeight));

	return manager;
}

void uniform_manager_free(UBOManager *self)
{
	VkDevice logicalDevice = gf3d_vgraphics_get_default_logical_device();
	vkUnmapMemory(logicalDevice, self->uboBuffersDeviceMemory);
	vkDestroyBuffer(logicalDevice, self->uboBuffer, NULL);
	vkFreeMemory(logicalDevice, self->uboBuffersDeviceMemory, NULL);
}

/**
 * @brief Gets the index number of an entity inside of a swapchain
 * @param self The pointer to the created UBOManager
 * @param entityID The ID of the requested entity
 * @param swapchainID The ID of the swapchain the entity is on
 * @returns The index number of the entity in the requested swapchain
 */
Uint32 uniforms_get_reference_index(UBOManager *self, Uint32 entityID, Uint32 swapchainID)
{
	return entityID + (self->numEntities * swapchainID);
}

Uint32 uniforms_get_reference_offset(UBOManager *self, Uint32 entityID, Uint32 swapchainFrameID)
{
	return uniforms_get_reference_index(self, entityID, swapchainFrameID) * uniforms_aligned_size();
}

UniformBufferObject *uniforms_get_local_reference(UBOManager *self, Uint32 entityID)
{
	UniformBufferObject *obj = self->currentUBOStates;
	Uint32 offset = uniforms_aligned_size() * entityID;
	return &obj[entityID];
	//return (self->currentUBOStates) + (entityID * uniforms_aligned_size());
}

Uint32 uniforms_size()
{
	return sizeof(UniformBufferObject);
}

void uniforms_flush(UBOManager *self, Uint32 swapchainImageID)
{
	Uint32 swapchainImageBeginningOffset = uniforms_get_reference_offset(self, 0, swapchainImageID);
	Uint32 uboBlockSize = uniforms_aligned_size() * self->numEntities;
	VkMappedMemoryRange memoryRange = {0};

	vkMapMemory
	(
		gf3d_vgraphics_get_default_logical_device(),
		self->uboBuffersDeviceMemory,
		swapchainImageBeginningOffset,
		uboBlockSize,
		0,
		(void **)&self->mappedUBOBuffers
	);
	{
		memcpy(self->mappedUBOBuffers, self->currentUBOStates, uboBlockSize);
		memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		memoryRange.size = uboBlockSize;
		memoryRange.offset = swapchainImageBeginningOffset;
		memoryRange.memory = self->uboBuffersDeviceMemory;
		memoryRange.pNext = 0;
		vkFlushMappedMemoryRanges(gf3d_vgraphics_get_default_logical_device(), 1, &memoryRange);
	}
	vkUnmapMemory(gf3d_vgraphics_get_default_logical_device(), self->uboBuffersDeviceMemory);
}
