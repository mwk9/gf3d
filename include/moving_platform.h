#ifndef __MOVING_PLATFORM__
#define __MOVING_PLATFORM__

#include "entity.h"

void extra_entity_setup();

Entity *entity_moving_platform_init(void *extraData);
void entity_moving_platform_update(Entity *self);

//don't feel like making another file so here this goesssss
Entity *entity_collectable_bread_init(Vector3D position);
void entity_collectable_bread_update(Entity *self);

Entity *entity_button_init(Vector3D position);
void entity_button_update(Entity *self);

void entity_collectable_key_init(Vector3D position);
void entity_collectable_key_update(Entity *self);

#endif //!__MOVING_PLATFORM__