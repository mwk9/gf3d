#ifndef __PLAYER__
#define __PLAYER__

#include <gf3d_vgraphics.h>
#include <gf3d_camera.h>
#include <gf3d_swapchain.h>
#include "entity.h"

Entity *entity_player_init(Entity *player, void *extraData);
void entity_player_update(Entity *player, void *extraData);
void entity_player_free(Entity *player, void *extraData);
void entity_player_draw(Entity *player, void *extraData);
Matrix4 *camera_get_view();

#endif //!__PLAYER__
