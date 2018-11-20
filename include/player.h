#ifndef __PLAYER__
#define __PLAYER__

#include "entity.h"

void entity_player_init(Entity *player, void *extraData);
void entity_player_update(Entity *player, void *extraData);
void entity_player_free(Entity *player, void *extraData);
void entity_player_draw(Entity *player, void *extraData);

#endif //!__PLAYER__
