#ifndef __MOVING_PLATFORM__
#define __MOVING_PLATFORM__

#include "entity.h"

Entity *entity_moving_platform_init(void *extraData);
void entity_moving_platform_update(Entity *self);

#endif //!__MOVING_PLATFORM__