#include "player.h"

Camera *camera;

float entity_player_normalize_movement(Uint8 positive, Uint8 negative)
{
	if (positive)
	{
		return 1.0f;
	}
	if (negative)
	{
		return -1.0f;
	}
	return 0.0f;
}

void entity_player_calculate_position(Entity *player)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	const Uint8 keyW = keys[SDL_SCANCODE_W];
	const Uint8 keyS = keys[SDL_SCANCODE_S];
	const Uint8 keyD = keys[SDL_SCANCODE_D];
	const Uint8 keyA = keys[SDL_SCANCODE_A];
	Vector3D deltaMovement = 
	{ 
		entity_player_normalize_movement(keyD, keyA) * -1.0f,
		entity_player_normalize_movement(keyW, keyS) * -1.0f,
		//player->acceleration.z
		0.0f
	};

	vector3d_scale(deltaMovement, deltaMovement, 0.1f);
	//Vector3D_add(&player->position, &player->position, &deltaMovement);
	Vector3D_add(&player->velocity, &player->velocity, &deltaMovement);
	gf3d_camera_move(camera, deltaMovement.x, deltaMovement.y, deltaMovement.z);
}

void entity_player_calculate_rotation(Entity *player)
{
	static Sint32 deltaX, deltaY, deltaZ;
	float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	const Uint8 keyW = keys[SDL_SCANCODE_W];
	const Uint8 keyS = keys[SDL_SCANCODE_S];
	const Uint8 keyD = keys[SDL_SCANCODE_D];
	const Uint8 keyA = keys[SDL_SCANCODE_A];

	SDL_GetRelativeMouseState(&deltaX, &deltaY);
	//slog("deltaX (%i) deltaY (%i)", deltaX, deltaY);
	yaw = ((float)deltaX) * 0.00004f;
	pitch = ((float)deltaY) * -0.00003f;
	/*if (keyS)
	{
		//yaw = 0.5f;
		if (player->lastKeyPressed != SDL_SCANCODE_S)
		{
			player->rotation.z = 1.0f;
			player->lastKeyPressed = SDL_SCANCODE_S;
		}
		slog("pressed s");
	}
	else if (keyA)
	{
		if (player->lastKeyPressed != SDL_SCANCODE_A)
		{
			player->rotation.z = 0.0f;
			player->lastKeyPressed = SDL_SCANCODE_A;
		}
		slog("pressed a");
	}
	else if (keyD)
	{

	}
	else if (keyW)
	{
		//yaw = 0.0f;
		player->rotation.y = 0.0f;
	}*/

	//vector3d_copy(player->rotation, vector3d(yaw, pitch, roll));
	gf3d_camera_turn(camera, pitch, yaw, roll);
}

void entity_player_init(Entity *player, void *extraData)
{
	if (!player)
	{
		return;
	}

	player->update = (void (*)(Entity *))entity_player_update;
	player->free = (void (*)(Entity *))entity_player_free;
	player->draw = (void (*)(Entity *))entity_player_draw;
	camera = gf3d_camera_init(windowWidth, windowHeight, &player->position, &player->rotation);
}

void entity_player_update(Entity *player, void *extraData)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	Uint8 keySpacebar = keys[SDL_SCANCODE_SPACE];
	if (keySpacebar)
	{
		player->acceleration.z = 0.05;
	}

	entity_player_calculate_position(player);
	entity_player_calculate_rotation(player);

	gf3d_camera_update(camera);
}

void entity_player_free(Entity *player, void *extraData)
{

}

void entity_player_draw(Entity *player, void *extraData)
{

}
