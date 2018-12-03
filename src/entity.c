#include "entity.h"
#include "ds_octree.h"
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
	slog("Entity system closed.");
}

void entity_system_init(Uint32 maxEntities)
{
	size_t i = 0;

	if (maxEntities <= 0)
	{
		slog("Error: Cannot initialize Entity manager for zero or negative entities.");
		return;
	}
	
	memset(&entityManager, 0, sizeof(EntityManager));
	entityManager.entityList = (Entity *)malloc(sizeof(Entity) * maxEntities);
	if (!entityManager.entityList)
	{
		slog("Error: Could not allocate memory of the entity list.");
		entity_system_close();
		return;
	}
	memset(entityManager.entityList, 0, sizeof(Entity) * maxEntities);
	entityManager.maxEntities = maxEntities;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		entityManager.entityList[i].id = i;
	}

	slog("Entity system initialized.");
	atexit(entity_system_close);
}

/*void entity_set_ubo(Entity *self)
{
	static UniformBufferObject localUBO;
	if (!self)
	{
		return;
	}

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
	gf3d_matrix_perspective(localUBO.proj, (float)(45 * GF3D_DEGTORAD), 1200.0f / 700.0f, 0.1f, 100.0f);
	localUBO.proj[1][1] *= -1;
	if (localUBO.proj[1][1] > 0)
	{
		localUBO.proj[1][1] *= -1;
	}
	memcpy(self->ubo, &localUBO, sizeof(Un))
}*/

Entity *entity_new()
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse == 0)
		{
			slog("Found a spot for an entity at index (%i)", i);
			memset(&entityManager.entityList[i], 0, sizeof(Entity));
			entityManager.entityList[i].id = i;
			entityManager.entityList[i].inUse = 1;
			//entityManager.entityList[i].model = (Model *)malloc(sizeof(Model));
			entityManager.entityList[i].canRotate = 1;
			entityManager.entityList[i].ubo = uniforms_get_local_reference(gf3d_vgraphics_get_uniform_buffer_manager(), i, 0);
			entityManager.entityList[i].scale = vector3d(1.0f, 1.0f, 1.0f);
			return &entityManager.entityList[i];
		}
	}

	slog("Error: Out of entity address for a new entity.");
	return NULL;
}

Entity *entity_new_at_id(Uint32 id)
{
	if (id < 0 || id >= MAX_ENTITY_NUM)
	{
		slog("Error: Trying to create an Entity with an invalid ID... range is from 0-%i.", MAX_ENTITY_NUM);
		return NULL;
	}

	if (entityManager.entityList[id].inUse == 0)
	{
		slog("Found a spot for an entity at index (%i)", id);
		memset(&entityManager.entityList[id], 0, sizeof(Entity));
		entityManager.entityList[id].id = id;
		entityManager.entityList[id].inUse = 1;
		//entityManager.entityList[i].model = (Model *)malloc(sizeof(Model));
		entityManager.entityList[id].canRotate = 1;
		entityManager.entityList[id].ubo = uniforms_get_local_reference(gf3d_vgraphics_get_uniform_buffer_manager(), id, 0);
		entityManager.entityList[id].scale = vector3d(1.0f, 1.0f, 1.0f);
		return &entityManager.entityList[id];
	}

	slog("Error: Entity with ID (%i) is already in use.", id);
	return NULL;
}

Entity *entity_load(char *modelFilename, int id)
{
	Entity *e;

	if (id < 0)
	{
		e = entity_new();
	}
	else
	{
		e = entity_new_at_id(id);
	}

	if (!e)
	{
		return NULL;
	}

	e->model = gf3d_model_load(modelFilename);
	e->shape = cube_new(e->position.x, e->position.y, e->position.z, 1.0f, 1.0f, 1.0f);
	memcpy(e->name, modelFilename, sizeof(TextLine));
	//entity_configure_render_pool(e);
	//e->ubo = uniforms_get_local_reference(gf3d_vgraphics_get_uniform_buffer_manager(), (Uint32)e->id);

	return e;
}

Entity *entity_load_from_file(char *filename)
{
	Entity *e = entity_new();
	char buffer[512];
	char holder[512];
	char c;
	int i = 0, n = 0;
	FILE *file = NULL;
	char *fileContents = "";
	size_t filesize = 0;

	if (!filename)
	{
		slog("Error: Trying to load an Entity from a NULL filename.");
		return NULL;
	}
	if (!e)
	{
		return NULL;
	}
	e->shape = cube_new(e->position.x, e->position.y, e->position.z, 1.0f, 1.0f, 1.0f);

	file = fopen(filename, "r");
	fseek(file, 0L, SEEK_END);
	filesize = ftell(file);
	rewind(file);
	fileContents = (char *)malloc(sizeof(char) * filesize);
	memset(fileContents, 0, sizeof(char) * filesize);

	c = fgetc(file);
	while (c != EOF)
	{
		fileContents[i++] = c;
		c = fgetc(file);
	}
	fileContents[i] = '\0';
	//slog("File contains:\n%s", fileContents);

	while (sscanf(fileContents, " %s\n%n", buffer, &n) == 1)
	{
		if (buffer[0] == '~')
		{
			//'~' denotes end of important info: text afterwards is ignored
			break;
		}
		fileContents += n;
		if (strcmp(buffer, "name:") == 0)
		{
			sscanf(fileContents, " %s\n%n", buffer, &n);
			fileContents += n;
			strncpy(e->name, buffer, sizeof(TextLine));
			slog("name is (%s)", e->name);
			continue;
		}
	}

	return e;
}

void entity_free(Entity *e)
{
	if (!e)
	{
		slog("Error: Cannot free an entity that is NULL.");
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
	Vector3D rotationAxisX = vector3d(1, 0, 0);
	Vector3D rotationAxisY = vector3d(0, 1, 0);
	Vector3D rotationAxisZ = vector3d(0, 0, 1);
	void *data;

	if (!self)
	{
		slog("Error: Cannot update an entity that is NULL.");
		return;
	}
	if (self->inUse == 0)
	{
		slog("Error: Cannot update an entity that is not in use.");
		return;
	}
	if (self->update)
	{
		self->update(self);
	}


	//gf3d_matrix_identity(self->ubo->model);

	//update position
	self->ubo->model[3][0] = self->position.x;
	self->ubo->model[3][1] = self->position.y;
	self->ubo->model[3][2] = self->position.z;

	//update scale
	//self->ubo->model[0][0] = self->scale.x;
	//self->ubo->model[1][1] = self->scale.y;
	//self->ubo->model[2][2] = self->scale.z;

	/*if (self->id == 0)
	{
		if (self->testNum == 0)
		{
			self->ubo->model[0][0] -= 0.05f;
			if (self->ubo->model[0][0] <= 0.01)
			{
				self->testNum = 1;
			}
		}
		else if (self->testNum == 1)
		{
			self->ubo->model[0][0] += 0.05f;
			if (self->ubo->model[0][0] >= 1)
			{
				self->testNum = 0;
			}
		}
	}*/
	
	if (!self->isStatic)
	{
		if (self->rotation.x != 0.0f)
		{
			gf3d_matrix_rotate(self->ubo->model, self->ubo->model, self->rotation.x, rotationAxisX);
			self->rotation.x = 0.0f;
		}
		if (self->rotation.y != 0.0f)
		{
			gf3d_matrix_rotate(self->ubo->model, self->ubo->model, self->rotation.y, rotationAxisY);
			self->rotation.y = 0.0f;
		}
		if (self->rotation.z != 0.0f)
		{
			gf3d_matrix_rotate(self->ubo->model, self->ubo->model, self->rotation.z, rotationAxisZ);
			self->rotation.z = 0.0f;
		}

		//gravity and physics stuff goes here
		if (self->useGravity)
		{
			if (self->acceleration.z > -0.5f)
			{
				self->acceleration.z -= 0.001f;
			}
		}
		vector3d_add(self->velocity, self->velocity, self->acceleration);
		vector3d_add(self->position, self->velocity, self->acceleration);
		if (self->shape)
		{
			self->shape->shape.cube.x = self->position.x;
			self->shape->shape.cube.y = self->position.y;
			self->shape->shape.cube.z = self->position.z;
		}
	}
	//if ()
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

/*void entity_set_draw_position(Entity *self, Vector3D position)
{
	int i = 0;
	void *data;

	if (!self)
	{
		slog("Warning: Trying to draw a null entity");
		return;
	}

	self->ubo->model[3][0] = 30.0;
	self->ubo->model[3][1] = 30.0;
	self->ubo->model[3][2] = 30.0;
	
	vkMapMemory(get_device(), get_uniform_buffers_memory()[i], 0, sizeof(self->ubo), 0, &data);
	memcpy(data, &(self->ubo), sizeof(self->ubo));
	vkUnmapMemory(get_device(), get_uniform_buffers_memory()[i]);
}*/

void entity_configure_render_pool(Entity *self)
{
	if (!self)
	{
		slog("Error: cannot configure renders for a null entity.");
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
	gf3d_model_draw((Uint32)self->id, bufferFrame, self->model, bufferFrame, commandBuffer);
	//gf3d_command_rendering_end(self->commandBuffer);
	//gf3d_vgraphics_render_end(self->bufferFrame);
}

void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse > 0)
		{
			if (entityManager.entityList[i].model != NULL)
			{
				entity_draw(&entityManager.entityList[i], bufferFrame, commandBuffer);
			}
		}
	}

	//entity_render_end_all();
}

void entity_scale(Entity *self, Vector3D scale)
{
	if (!self)
	{
		slog("Error: Attempting to scale a NULL Entity.");
		return;
	}

	self->scale.x = scale.x;
	self->scale.y = scale.y;
	self->scale.z = scale.z;

	self->ubo->model[0][0] = self->scale.x;
	self->ubo->model[1][1] = self->scale.y;
	self->ubo->model[2][2] = self->scale.z;
}

Entity *entity_get_by_id(Uint32 id)
{
	if (id < 0 || id >= MAX_ENTITY_NUM)
	{
		slog("Error: Requesting an Entity that is out of range.");
		return NULL;
	}

	return &entityManager.entityList[id];
}

void TEST_entity_collides_with_floor(Entity *self, Entity *floor)
{
	if (!self || !floor)
	{
		return;
	}
	if (self == floor)
	{
		return;
	}

	if (cube_in_cube(&self->shape->shape.cube, &floor->shape->shape.cube))
	{
		self->acceleration = vector3d(0.0f, 0.0f, 0.0f);
		self->useGravity = 0;
	}
	else
	{
		self->useGravity = 1;
	}
}

void TEST_all_entities_collide_with_floor(Entity *floor)
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse)
		{
			TEST_entity_collides_with_floor(&entityManager.entityList[i], floor);
		}
	}
}
