#ifndef __UI__
#define __UI__

#include <stdint.h>
#include <vulkan/vulkan_core.h>
#include <gf3d_vgraphics.h>
#include <gf3d_swapchain.h>
#include <gf3d_model.h>
#include <gf3d_pipeline.h>
#include <gf3d_texture.h>
#include "simple_logger.h"

typedef struct ui_screenspace_s
{
	VkDeviceSize size;
	VkBuffer buffer;
	VkDeviceMemory memory;
}UIScreenspace;

typedef struct ui_pipeline_s
{
	Pipeline *pipeline;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet *descriptorSets;
	VkDescriptorPool descriptorPool;
	uint32_t descriptorSetCount;
	uint32_t screenWidth, screenHeight;
	Texture *interfaceTexture;
	UIScreenspace screenspace;
}UIPipeline;

UIPipeline *ui_pipeline_init(Texture *interfaceTexture, uint32_t screenWidth, uint32_t screenHeight);
void ui_pipeline_renderpass(UIPipeline *self, VkCommandBuffer buffer, uint32_t frame);

#endif //!__UI__