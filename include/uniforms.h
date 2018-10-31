#ifndef __UNIFORMS__
#define __UNIFORMS__

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <string.h>
#include "gf3d_matrix.h"
#include "simple_logger.h"
#include "entity.h"

typedef struct uboManager_s
{
	Uint32 numEntities;
	Uint32 numSwapchainImages;
	Uint32 numUBOs;

	VkDeviceSize uboBuffersSize;
	VkBuffer uboBuffer;
	VkDeviceMemory uboBuffersDeviceMemory;

	UniformBufferObject *currentUBOStates;
	UniformBufferObject *mappedUBOBuffers;
}UBOManager;

UBOManager * ubo_manager_init(Uint32 numEntities, Uint32 numSwapchainImages, int renderWidth, int renderHeight);

/**
 * @brief Frees the UBOManager
 */
void uniform_manager_free(UBOManager *self);
Uint32 uniforms_get_reference_offset(UBOManager *self, Uint32 entityID, Uint32 swapchainFrameID);

/**
 * @brief Finds the UBO of a specific entity in memory of the UBOManager
 * @param self A pointer to the created UBOManager
 * @param entityID The ID of the specific entity to grab the UBO of
 * @returns A pointer to the current UBO state of the requested entity
 */
UniformBufferObject *uniforms_get_local_reference(UBOManager *self, Uint32 entityID);

/**
 * @brief Gets the size of a UBO
 * @returns The sizeof a UniformBufferObject
 */
Uint32 uniforms_size();
void uniforms_flush(UBOManager *self, Uint32 swapchainImageID);

#endif // !__UNIFORMS__
