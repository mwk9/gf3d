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
	bread->update = (void(*)(Entity *))entity_collectable_bread_update;
}

void entity_collectable_bread_update(Entity *self)
{
	if (!self)
	{
		return;
	}

	self->rotation.x = 0.5f;
}
