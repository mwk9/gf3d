#include "player.h"

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
		entity_player_normalize_movement(keyD, keyA),
		entity_player_normalize_movement(keyW, keyS),
		0.0f
	};

	//vec3_scale(deltaMovement, deltaMovement, playerVelocity);
}
