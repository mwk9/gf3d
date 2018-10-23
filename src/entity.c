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

	//gravity and physics stuff goes here
}

void entity_update_all()
{
	int i = 0;

	for (i = 0; entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse == 1)
		{
			entity_update(&entityManager.entityList[i]);
		}
	}
}

void entity_set_draw_position(Entity *self, Vector3D position)
{
	if (!self)
	{
		slog("Warning: Trying to draw a null entity");
		return;
	}

	//self->ubo.model[3][0]
}

void entity_draw(Entity *self)
{

}
