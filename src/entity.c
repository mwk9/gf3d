#include "entity.h"
#include "gf3d_vgraphics.h"

typedef struct entityManager_s
{
	Uint32 maxEntities;
	Entity *entityList;
	Uint64 increment;
}EntityManager;

static EntityManager entityManager = { 0, NULL, 0 };

void entity_system_close()
{
	int i = 0;

	if (entityManager.entityList != NULL)
	{
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			entity_free(&entityManager.entityList[i]);
		}
		free(entityManager.entityList);
	}
	memset(&entityManager, 0, sizeof(entityManager));
	slog("Entity system closed");
}

void entity_system_init(Uint32 maxEntities)
{
	if (maxEntities <= 0)
	{
		slog("Error: Cannot initialize Entity manager for zero or negative entities");
		return;
	}
	
	memset(&entityManager, 0, sizeof(EntityManager));
	entityManager.entityList = (Entity *)malloc(sizeof(Entity) * maxEntities);
	if (!entityManager.entityList)
	{
		slog("Error: Could not allocate memory of the entity list");
		entity_system_close();
		return;
	}
	memset(entityManager.entityList, 0, sizeof(Entity) * maxEntities);
	entityManager.maxEntities = maxEntities;

	slog("Entity system initialized");
	atexit(entity_system_close);
}

Entity *entity_new()
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse == 0)
		{
			slog("Found a spot for an entity at index (%i)", i);
			memset(&entityManager.entityList[i], 0, sizeof(Entity));
			entityManager.entityList[i].id = entityManager.increment++;
			entityManager.entityList[i].inUse = 1;

			//entityManager.entityList[i].model = (Model *)malloc(sizeof(Model));

			return &entityManager.entityList[i];
		}
	}

	slog("Error: Out of entity address for a new entity");
	return NULL;
}

Entity *entity_load(char *modelFilename)
{
	Entity *e;

	e = entity_new();
	if (!e)
	{
		return NULL;
	}

	e->model = gf3d_model_load(modelFilename);
	//entity_configure_render_pool(e);

	return e;
}

void entity_free(Entity *e)
{
	if (!e)
	{
		slog("Error: Cannot free an entity that is NULL");
		return;
	}
	//extra free behaviour, if any
	if (e->free)
	{
		e->free(e);
	}

	memset(e, 0, sizeof(Entity));
}

void entity_free_all()
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		entity_free(&entityManager.entityList[i]);
	}
}

void entity_update(Entity *self)
{
	if (!self)
	{
		slog("Error: Cannot update an entity that is NULL");
		return;
	}
	if (self->inUse == 0)
	{
		slog("Error: Cannot update an entity that is not in use");
		return;
	}
	if (self->update)
	{
		self->update(self);
	}

	gf3d_matrix_identity(self->ubo.model);
	gf3d_matrix_make_translation(self->ubo.model, self->position);

	gf3d_matrix_rotate(self->ubo.model, self->ubo.model, 0.05f, self->rotation);
	//gravity and physics stuff goes here
}

void entity_update_all()
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse == 1)
		{
			entity_update(&entityManager.entityList[i]);
		}
	}
}

void entity_set_draw_position(Entity *self, Vector3D position)
{
	int i = 0;
	void *data;

	if (!self)
	{
		slog("Warning: Trying to draw a null entity");
		return;
	}

	self->ubo.model[3][0] = 30.0;
	self->ubo.model[3][1] = 30.0;
	self->ubo.model[3][2] = 30.0;
	
	vkMapMemory(get_device(), get_uniform_buffers_memory()[i], 0, sizeof(self->ubo), 0, &data);
	memcpy(data, &(self->ubo), sizeof(self->ubo));
	vkUnmapMemory(get_device(), get_uniform_buffers_memory()[i]);
}

void entity_configure_render_pool(Entity *self)
{
	if (!self)
	{
		slog("Error: cannot configure renders for a null entity");
		return;
	}

	//self->bufferFrame = gf3d_vgraphics_render_begin();
	//self->commandBuffer = gf3d_command_rendering_begin(self->bufferFrame);
}

void entity_render_end(Entity *self, Uint8 *keys, float direction)
{
	if (!self)
	{
		return;
	}

	gf3d_command_rendering_end(self->commandBuffer);
	gf3d_vgraphics_render_end(self->bufferFrame, keys, direction);
}

void entity_render_end_all(Uint8 *keys, float direction)
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse)
		{
			if (entityManager.entityList[i].model)
			{
				entity_render_end(&entityManager.entityList[i], keys, direction);
			}
		}
	}
}

void entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	if (!self)
	{
		return;
	}
	
	//entity_configure_render_pool(self);
	gf3d_model_draw(self->model, bufferFrame, commandBuffer);
	//gf3d_command_rendering_end(self->commandBuffer);
	//gf3d_vgraphics_render_end(self->bufferFrame);
}

void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse)
		{
			if (entityManager.entityList[i].model)
			{
				entity_draw(&entityManager.entityList[i], bufferFrame, commandBuffer);
			}
		}
	}

	//entity_render_end_all();
}
