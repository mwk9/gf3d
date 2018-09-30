#ifndef __ENTITY__
#define __ENTITY__

#include <stdio.h>
#include "gf3d_vector.h"
#include "gf3d_text.h"
#include "simple_logger.h"

typedef struct entity_s
{
	short unsigned int inUse;
	Uint64 id;
	TextLine name;

	//physics
	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;

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

#endif // !__ENTITY__
