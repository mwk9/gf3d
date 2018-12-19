#include "moving_platform.h"

Entity *entity_moving_platform_init(void *extraData)
{
	Entity *platform = NULL;

	platform = entity_load_from_file("def/s_m_platform.def");
	platform->isStatic = 1;
	platform->update = (void (*)(Entity *))entity_moving_platform_update;
}

void entity_moving_platform_update(Entity *self)
{
	static int test = 0;

	if (!self)
	{
		slog("Error: moving platform was null");
		return;
	}

	if (test == 0)
	{
		self->position.x -= 0.05f;
		if (self->position.x <= -10.0f)
		{
			test = 1;
		}
	}
	else if (test == 1)
	{
		self->position.x += 0.05f;
		if (self->position.x >= 10.0f)
		{
			test = 0;
		}
	}
}

Entity *entity_collectable_bread_init(void *extraData)
{
	Entity *bread = NULL;

	bread = entity_load_from_file("def/collectable.def");
	bread->isStatic = 1;
	bread->useGravity = 0;
	bread->update = (void(*)(Entity *))entity_collectable_bread_update;
}

void entity_collectable_bread_collide_with_player(Entity *self, Uint32 playerID)
{
	Entity *player = NULL;

	if (!self)
	{
		return;
	}
	if (playerID < 0 || playerID >= MAX_ENTITY_NUM)
	{
		return;
	}

	player = entity_get_at_location(playerID);
	if (!player)
	{
		return;
	}

	if (cube_in_cube(&self->shape->shape.cube, &player->shape->shape.cube))
	{
		slog("Player touched collectable!");
	}
}

void entity_collectable_bread_update(Entity *self)
{
	if (!self)
	{
		return;
	}

	self->rotation.z = 0.025f;
	if (self->shape)
	{
		self->shape->shape.cube.x = self->position.x;
		self->shape->shape.cube.y = self->position.y;
		self->shape->shape.cube.z = self->position.z;
	}
	entity_collectable_bread_collide_with_player(self, 50);
}
