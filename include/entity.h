#ifndef __ENTITY__
#define __ENTITY__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gf3d_vector.h"
#include "gf3d_text.h"
#include "gf3d_matrix.h"
#include "gf3d_model.h"
#include "simple_logger.h"

#define MAX_ENTITY_NUM 1024

typedef struct entity_s
{
	short unsigned int inUse;
	Uint32 id;
	TextLine name;

	//physics
	Vector3D position;
	Vector3D rotation;
	Vector3D velocity;
	Vector3D acceleration;
	Uint8 useGravity;
	Uint8 testNum;

	//Mesh data
	Uint32 bufferFrame;
	VkCommandBuffer commandBuffer;
	UniformBufferObject *ubo;
	Model *model;

	//function pointers for logic
	void(*draw)(struct entity_s *self);
	void(*update)(struct entity_s *self);
	void(*die)(struct entity_s *self);
	void(*free)(struct entity_s *self);
	
}Entity;

/**
 * @brief Closes the Entity System, automatically called in entity_system_init atexit()
 */
void entity_system_close();

/**
 * @brief Initializes the Entity Manager
 * @param maxEntities The maximum number of entities that can exist at once
 */
void entity_system_init(Uint32 maxEntities);

/**
 * @brief Finds an spot in the Entity Manager that is not in use and returns its address
 * @returns An address in memory for the new entity if successful; NULL if failed
 */
Entity *entity_new();

/**
 * @brief Creates a new entity with specified attributes
 * @param modelFilename The filename of the model to assign to the entity
 * @returns A pointer to the new entity
 */
Entity *entity_load(char *modelFilename);

/**
 * @brief Recycles an entity slot back to the Entity Manager
 * @param e The entity to free
 */
void entity_free(Entity *e);

/**
 * @brief Recycles all entities in the Entity Manager without closing the system
 */
void entity_free_all();

/**
 * @brief Runs generic updates on an update, plus any extra function if an entity has one
 * @param self The entity to update
 */
void entity_update(Entity *self);

/**
 * @brief Runs entity_update on every existing entity
 */
void entity_update_all();

/**
 * TODO: comment this function
 */
void entity_set_draw_position(Entity *self, Vector3D position);

void entity_configure_render_pool(Entity *self);
void entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer);
void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

#endif // !__ENTITY__
