#include "ui.h"

void ui_pipeline_renderpass(UIPipeline *self, VkCommandBuffer buffer, uint32_t frame)
{
	const VkDescriptorSet *descSet = &self->descriptorSets[frame];
	vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, self->pipeline->pipeline);
	vkCmdBindDescriptorSets
	(
		buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		self->pipeline->pipelineLayout,
		0,
		1,
		descSet,
		0,
		NULL
	);
	vkCmdDraw(buffer, 4, 1, 0, 0);
	vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gf3d_vgraphics_get_graphics_pipeline()->pipeline);
}

void ui_pipeline_descriptor_set_pool_init(UIPipeline *self)
{
	VkDescriptorPoolSize poolSize[1] = { 0 };
	VkDescriptorPoolCreateInfo poolInfo = { 0 };

	if (!self)
	{
		slog("Error: Cannot create descriptor pools of a NULL UI Pipeline.");
		return;
	}

	slog("Attempting to make descriptor pools of size (%i)", gf3d_swapchain_get_swap_image_count());
	poolSize[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize[0].descriptorCount = gf3d_swapchain_get_swap_image_count();
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = poolSize;
	poolInfo.maxSets = gf3d_swapchain_get_swap_image_count();

	if (vkCreateDescriptorPool(gf3d_vgraphics_get_default_logical_device(), &poolInfo, NULL, &self->descriptorPool) != VK_SUCCESS)
	{
		slog("Error: Failed to create descriptor pool.");
		return;
	}
}

void ui_pipeline_descriptor_set_init(UIPipeline *self)
{
	VkDescriptorSetLayout *layouts = NULL;
	VkDescriptorSetAllocateInfo allocInfo = { 0 };
	VkWriteDescriptorSet descWrite[1] = { 0 };
	VkDescriptorImageInfo imageInfo = { 0 };
	int i = 0;

	if (!self)
	{
		slog("Error: Could not create descriptor sets from a NULL UI Pipeline.");
		return;
	}

	layouts = (VkDescriptorSetLayout *)malloc(sizeof(VkDescriptorSetLayout) * gf3d_swapchain_get_swap_image_count());
	if (!layouts)
	{
		slog("Error: Could not allocate memory for descriptor set layouts.");
		return;
	}
	memset(layouts, 0, sizeof(VkDescriptorSetLayout) * gf3d_swapchain_get_swap_image_count());
	for (i = 0; i < gf3d_swapchain_get_swap_image_count(); i++)
	{
		memcpy(&layouts[i], &self->descriptorSetLayout, sizeof(VkDescriptorSetLayout));
	}

	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = self->descriptorPool;
	allocInfo.descriptorSetCount = gf3d_swapchain_get_swap_image_count();
	allocInfo.pSetLayouts = layouts;

	self->descriptorSets = (VkDescriptorSet *)malloc(sizeof(VkDescriptorSet) * gf3d_swapchain_get_swap_image_count());
	if (!self->descriptorSets)
	{
		slog("Error: Could not allocate memory for descriptor sets.");
		free(layouts);
		return;
	}
	memset(self->descriptorSets, 0, sizeof(VkDescriptorSet) * gf3d_swapchain_get_swap_image_count());
	if (vkAllocateDescriptorSets(gf3d_vgraphics_get_default_logical_device(), &allocInfo, self->descriptorSets) != VK_SUCCESS)
	{
		slog("Error: Failed to allocate descriptor sets.");
		free(layouts);
		free(self->descriptorSets);
		return;
	}

	self->descriptorSetCount = gf3d_swapchain_get_swap_image_count();
	for (i = 0; i < gf3d_swapchain_get_swap_image_count(); i++)
	{
		slog("Updating descriptor sets for index (%i)", i);

		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = self->interfaceTexture->textureImageView;
		imageInfo.sampler = self->interfaceTexture->textureSampler;

		descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite[0].dstSet = self->descriptorSets[i];
		descWrite[0].dstBinding = 1;
		descWrite[0].dstArrayElement = 0;
		descWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descWrite[0].descriptorCount = 1;
		descWrite[0].pImageInfo = &imageInfo;
		descWrite[0].pTexelBufferView = NULL;
		descWrite[0].pNext = NULL;

		vkUpdateDescriptorSets(gf3d_vgraphics_get_default_logical_device(), 1, descWrite, 0, NULL);
	}
}

void ui_pipeline_graphics_descriptor_set_layout(VkDescriptorSetLayout *layout)
{
	static VkDescriptorSetLayoutCreateInfo layoutInfo = { 0 };
	static VkDescriptorSetLayoutBinding bindings[1] = { 0 };
	memset(bindings, 0, sizeof(bindings));

	VkDescriptorSetLayoutBinding *blockTextureBinding = &bindings[0];
	{
		blockTextureBinding->binding = 1;
		blockTextureBinding->descriptorCount = 1;
		blockTextureBinding->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		blockTextureBinding->pImmutableSamplers = NULL;
		blockTextureBinding->stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = bindings;

	if (vkCreateDescriptorSetLayout(gf3d_vgraphics_get_default_logical_device(), &layoutInfo, NULL, layout) != VK_SUCCESS)
	{
		slog("Error: Failed to create descriptor set layout!");
		return;
	}
}

void ui_pipeline_graphics_ui_init(UIPipeline *self)
{
	static VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
	static VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;

	slog("ui_pipeline_graphics_ui_init: Clearing data structures...");
	memset(&vertexInputStateCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
	memset(&inputAssemblyStateCreateInfo, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));

	slog("ui_pipeline_graphics_ui_init: Configuring VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO...");
	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputStateCreateInfo.pVertexBindingDescriptions = NULL;
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = NULL;

	slog("ui_pipeline_graphics_ui_init: Configuring VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO...");
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	ui_pipeline_graphics_descriptor_set_layout(&self->descriptorSetLayout);

	slog("ui_pipeline_graphics_ui_init: Building pipeline...");
	self->pipeline = gf3d_pipeline_graphics_load
	(
		gf3d_vgraphics_get_default_logical_device(),
		"shaders/ui.vert.spv",
		"shaders/ui.frag.spv",
		gf3d_vgraphics_get_view_extent()
	); //maybe come back to this?

	if (!self->pipeline)
	{
		slog("Error: Failed to build UI Pipeline.");
		return;
	}
}

UIPipeline *ui_pipeline_init(Texture *interfaceTexture, uint32_t screenWidth, uint32_t screenHeight)
{
	UIPipeline *pipeline = NULL;

	if (!interfaceTexture)
	{
		slog("Error: Interface Texture was NULL.");
		return NULL;
	}
	
	pipeline = malloc(sizeof(UIPipeline));
	if (!pipeline)
	{
		slog("Error: Could not allocate memory for UI Pipeline.");
		return NULL;
	}
	memset(pipeline, 0, sizeof(UIPipeline));
	pipeline->screenWidth = gf3d_vgraphics_get_view_extent().width;
	pipeline->screenHeight = gf3d_vgraphics_get_view_extent().height;
	pipeline->interfaceTexture = interfaceTexture;

	ui_pipeline_graphics_ui_init(pipeline);
	ui_pipeline_descriptor_set_pool_init(pipeline);
	ui_pipeline_descriptor_set_init(pipeline);

	return pipeline;
}
