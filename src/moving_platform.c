#include "moving_platform.h"
#include "sound.h"

static Entity *player = NULL;
static Uint8 playerOnCube = 0;

void extra_entity_setup()
{
	player = entity_get_by_id(50);
}

Entity *entity_moving_platform_init(void *extraData)
{
	Entity *platform = NULL;

	platform = entity_load_from_file("def/s_m_platform.def");
	platform->isStatic = 0;
	platform->useGravity = 0;
	platform->position.y = -5.0f;
	platform->position.z = -3.0f;
	platform->update = (void (*)(Entity *))entity_moving_platform_update;

	return platform;
}

void entity_moving_platform_collide_with_player(Entity *self, Uint32 playerID)
{
	//Entity *player = NULL;

	if (!self)
	{
		return;
	}
	if (playerID < 0 || playerID >= MAX_ENTITY_NUM)
	{
		return;
	}

	//player = entity_get_by_id(playerID);
	if (!player)
	{
		return;
	}

	if (cube_in_cube(&self->shape->shape.cube, &player->shape->shape.cube))
	{
		player->acceleration = vector3d(0.0f, 0.0f, 0.0f);
		player->useGravity = 0;
		player->onGround = 1;
		playerOnCube = 1;
	}
	else
	{
		playerOnCube = 0;
	}
}

void entity_moving_platform_update(Entity *self)
{
	static int test = 0;
	//Entity *player = entity_get_by_id(50);

	if (!self)
	{
		slog("Error: moving platform was null");
		return;
	}

	self->useGravity = 0;

	if (test == 0)
	{
		self->position.x -= 0.05f;
		if (playerOnCube > 0)
		{
			player->position.x -= 0.05f;
			player->velocity.x -= 0.05f;
		}
		if (self->position.x <= -10.0f)
		{
			test = 1;
		}
	}
	else if (test == 1)
	{
		self->position.x += 0.05f;
		if (playerOnCube > 0)
		{
			player->position.x += 0.05f;
			player->velocity.x += 0.05f;
		}
		if (self->position.x >= 10.0f)
		{
			test = 0;
		}
	}
	entity_moving_platform_collide_with_player(self, 50);
}

Entity *entity_collectable_bread_init(Vector3D position)
{
	Entity *bread = NULL;

	bread = entity_load_from_file("def/collectable.def");
	bread->isStatic = 1;
	bread->useGravity = 0;
	bread->position.x = position.x;
	bread->position.y = position.y;
	bread->position.z = position.z;
	bread->shape->shape.cube.x = bread->position.x;
	bread->shape->shape.cube.y = bread->position.y;
	bread->shape->shape.cube.z = bread->position.z;
	bread->update = (void(*)(Entity *))entity_collectable_bread_update;

	return bread;
}

void entity_collectable_bread_collide_with_player(Entity *self, Uint32 playerID)
{
	//Entity *player = NULL;
	static int numCollectables = 0;

	if (!self)
	{
		return;
	}
	if (playerID < 0 || playerID >= MAX_ENTITY_NUM)
	{
		return;
	}

	//player = entity_get_by_id(playerID);
	if (!player)
	{
		return;
	}

	if (cube_in_cube(&self->shape->shape.cube, &player->shape->shape.cube))
	{
		slog("Player touched collectable!");
		sound_play_get_by_filepath("audio/sfx/eat.ogg", 0, 0, -1, 0);
		numCollectables++;
		slog("Player now has (%i) pieces of bread", numCollectables);
		entity_free(self);
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

Entity *entity_button_init(Vector3D position)
{
	Entity *button = NULL;

	button = entity_load_from_file("def/button.def");
	button->isStatic = 1;
	button->useGravity = 0;
	button->position.x = position.x;
	button->position.y = position.y;
	button->position.z = position.z;
	button->shape->shape.cube.x = button->position.x;
	button->shape->shape.cube.y = button->position.y;
	button->shape->shape.cube.z = button->position.z;
	button->update = (void(*)(Entity *))entity_button_update;

	return button;
}

void entity_button_collide_with_player(Entity *self)
{
	if (!self)
	{
		return;
	}
	if (!self->inUse)
	{
		return;
	}
	if (!player)
	{
		return;
	}

	if (cube_in_cube(&self->shape->shape.cube, &player->shape->shape.cube))
	{
		slog("Player touched button!");
		sound_play_get_by_filepath("audio/sfx/jump.ogg", 0, 0, -1, 0);
		entity_free(self);
		entity_collectable_bread_init(vector3d(2.0f, 2.0f, -3.0f));
	}
}

void entity_button_update(Entity *self)
{
	if (!self)
	{
		return;
	}
	if (!self->inUse)
	{
		return;
	}

	/*if (self->shape)
	{
		self->shape->shape.cube.x = self->position.x;
		self->shape->shape.cube.y = self->position.y;
		self->shape->shape.cube.z = self->position.z;
	}*/

	entity_button_collide_with_player(self);
}
