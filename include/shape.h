#ifndef __SHAPE__
#define __SHAPE__

#include <stdlib.h>
#include "gf3d_vector.h"
#include "simple_logger.h"

typedef struct rect_s
{
	float x, y, w, h;
}Rect;

typedef struct cube_s
{
	float x, y, z, w, h, d;
}Cube;

typedef struct sphere_s
{
	float x, y, z, r;
}Sphere;

typedef enum
{
	ST_RECT,
	ST_CUBE,
	ST_SPHERE
}ShapeType;

typedef struct shape_s
{
	ShapeType type;
	union
	{
		Rect rect;
		Cube cube;
		Sphere sphere;
	}s;
}Shape;

/**
 * @brief Allocates memory for a new Rect
 * @param x The x position of the top-left of the Rect
 * @param y The y position of the top-left of the Rect
 * @param width The width of the Rect
 * @param height The height of the Rect
 * @returns A pointer to the new Rect; NULL if could not allocate memory
 */
Rect * rect_new(float x, float y, float width, float height);

/**
 * @brief Allocates memory for a new Cube
 * @param x The x position of the top-left of the Cube
 * @param y The y position of the top-left of the Cube
 * @param z The z position of the top-left of the Cube
 * @param width The width of the Cube
 * @param height The height of the Cube
 * @param depth The depth of the Cube
 * @returns A pointer to the new Cube; NULL if could not allocate memory
 */
Cube * cube_new(float x, float y, float z, float width, float height, float depth);

/**
 * @brief Allocates memory for a new Sphere
 * @param x The x position of the center of the Sphere
 * @param y The y position of the center of the Sphere
 * @param z The z position of the center of the Sphere
 * @param radius The radius of the Sphere
 * @returns A pointer to the new Sphere; NULL if could not allocate memory
 */
Sphere * sphere_new(float x, float y, float z, float radius);

/**
 * @brief Checks to see if a point is inside of a Rect
 * @param x The x value of the point
 * @param y The y value of the point
 * @param rect The rect to compare the point to
 * @returns 1 if collision; 0 if no collision or Rect was NULL
 */
Uint8 point_in_rect(float x, float y, Rect *rect);

/**
 * @brief Checks to see if two Rects are colliding
 * @param a The first Rect to check
 * @param b the second Rect to check
 * @returns 1 if collision; 0 if no collision or either Rect was NULL
 */
Uint8 rect_in_rect(Rect *a, Rect *b);

/**
 * @brief Checks to see if two Cubes are colliding
 * @param a The first Cube to check
 * @param b the second Cube to check
 * @returns 1 if collision; 0 if no collision or either Cube was NULL
 */
Uint8 cube_in_cube(Cube *a, Cube *b);

Uint8 point_in_sphere(float x, float y, Sphere *sphere);
Uint8 sphere_in_sphere(Sphere *a, Sphere *b);

#endif // !__SHAPE__
