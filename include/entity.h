#ifndef __ENTITY__
#define __ENTITY__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gf3d_vector.h"
#include "gf3d_text.h"
#include "gf3d_matrix.h"
#include "gf3d_model.h"
#include "shape.h"
#include "multitree.h"
#include "simple_logger.h"

#define MAX_CHARS 32
#define MAX_ENTITY_NUM 1024

typedef struct entity_s
{
	short unsigned int inUse;
	Uint32 id;
	TextLine name;

	//physics
	Vector3D position;
	Vector3D rotation;
	Uint8 canRotate;
	int lastKeyPressed;
	Vector3D velocity;
	Vector3D acceleration;
	Vector3D scale;
	float speed;
	Uint8 useGravity;
	Uint8 testNum;
	Shape *shape;
	Uint8 isStatic;
	Uint8 onGround;

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
	void(*touch)(struct entity_s *self, struct entity_s *other);
	
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
 * @brief Finds a spot in the Entity Manager that is not in use and returns its address
 * @returns An address in memory for the new Entity if successful; NULL if failed
 */
Entity *entity_new();

/**
 * @brief Finds a requested spot in the Entity Manager and returns its address
 * @param id The ID of the requested spot for an Entity
 * @returns The requested address in memory for the new Entity if successful; NULL if failed
 */
Entity *entity_new_at_id(Uint32 id);

/**
 * @brief Creates a new entity with specified attributes
 * @param modelFilename The filename of the model to assign to the entity
 * @param id Specifies if a Entity should be loaded with a specfic ID; -1 to use next available address
 * @returns A pointer to the new entity if successful; NULL if error
 */
Entity *entity_load(char *modelFilename, int id);

/**
 * @brief Creates a new entity with specified attributes from a defintion file
 * @param filename The name of the text definition file to load the Entity from
 * @returns A pointer to the new entity if successful; NULL if error
 */
Entity *entity_load_from_file(char *filename);

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

/**
 * @brief Scales an entity. TODO: scaling one or two axises only is a little buggy...
 * @param self The Entity to scale
 * @param scale A Vector3D of the x, y, and z amount to scale
 */
void entity_scale(Entity *self, Vector3D scale);

/**
 * @brief Returns an Entity by its ID
 * @param id The ID of the requested Entity
 * @returns A pointer to the Entity if successful; NULL if unsuccessful
 */
Entity *entity_get_by_id(Uint32 id);

void TEST_all_entities_collide_with_floor(Entity *floor);

#endif // !__ENTITY__
