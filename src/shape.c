#include "shape.h"

typedef struct shapeManager_s
{
	Uint64 increment;
	Shape *shapeList;
	Uint32 maxShapes;
}ShapeManager;

static ShapeManager shapeManager = { 0, NULL, 0 };

void shape_system_close()
{
	int i = 0;

	if (shapeManager.shapeList != NULL)
	{
		for (i = 0; i < shapeManager.maxShapes; i++)
		{
			shape_free(&shapeManager.shapeList[i]);
		}
		free(shapeManager.shapeList);
	}
	memset(&shapeManager, 0, sizeof(ShapeManager));
	slog("Shape system closed.");
}

void shape_system_init(Uint32 maxShapes)
{
	if (maxShapes <= 0)
	{
		slog("Error: Cannot initialize Shape manager for zero or negative shapes.");
		return;
	}

	memset(&shapeManager, 0, sizeof(ShapeManager));
	shapeManager.shapeList = (Shape *)malloc(sizeof(Shape) * maxShapes);
	if (!shapeManager.shapeList)
	{
		slog("Error: Could not allocate memory for the Shape list.");
		shape_system_close();
		return;
	}
	memset(shapeManager.shapeList, 0, sizeof(Shape) * maxShapes);
	shapeManager.maxShapes = maxShapes;

	slog("Shape system initialized.");
	atexit(shape_system_close);
}

Shape * shape_new()
{
	int i = 0;

	for (i = 0; i < shapeManager.maxShapes; i++)
	{
		if (shapeManager.shapeList[i].inUse == 0)
		{
			slog("Found a spot for an Shape at index (%i)", i);
			memset(&shapeManager.shapeList[i], 0, sizeof(Shape));
			shapeManager.shapeList[i].inUse = 1;
			return &shapeManager.shapeList[i];
		}
	}

	slog("Error: Out of memory for a new Shape.");
	return NULL;
}

void shape_free(Shape *shape)
{
	if (!shape)
	{
		slog("Error: Cannot free a Shape that is NULL.");
		return;
	}

	shape->inUse = 0;
	memset(shape, 0, sizeof(Shape));
}

Shape * rect_new(float x, float y, float width, float height)
{
	Shape *s = NULL;
	//Rect *r = NULL;
	//r = (Rect *)malloc(sizeof(Rect));
	s = shape_new();

	if (!s)
	{
		//slog("Error: could not allocate space for a new Rect");
		return NULL;
	}
	//memset(r, 0, sizeof(Rect));

	s->type = ST_RECT;
	s->shape.rect.x = x;
	s->shape.rect.y = y;
	s->shape.rect.w = width;
	s->shape.rect.h = height;
	return s;
}

Shape * cube_new(float x, float y, float z, float width, float height, float depth)
{
	Shape *s = NULL;
	s = shape_new();
	//Cube *c = NULL;
	//c = (Cube *)malloc(sizeof(Cube));
	if (!s)
	{
		//slog("Error: Could not allocate space for a new Cube.");
		return NULL;
	}
	//memset(c, 0, sizeof(Cube));
	
	s->type = ST_CUBE;
	s->shape.cube.x = x;
	s->shape.cube.y = y;
	s->shape.cube.z = z;
	s->shape.cube.w = width;
	s->shape.cube.h = height;
	s->shape.cube.d = depth;
	return s;
}

Shape * sphere_new(float x, float y, float z, float radius)
{
	Shape *s = NULL;
	s = shape_new();
	//Sphere *s = NULL;
	//s = (Sphere *)malloc(sizeof(Sphere));
	if (!s)
	{
		//slog("Error: Could not allocate space for a new Sphere.");
		return NULL;
	}
	//memset(s, 0, sizeof(Sphere));
	s->type = ST_SPHERE;
	s->shape.sphere.x = x;
	s->shape.sphere.y = y;
	s->shape.sphere.z = z;
	s->shape.sphere.r = radius;
	return s;
}

Uint8 point_in_rect(float x, float y, Rect *rect)
{
	if (!rect)
	{
		return 0;
	}

	if (x >= rect->x)
		if (x <= rect->x + rect->w)
			if (y >= rect->y)
				if (y <= rect->y + rect->h)
					return 1;
	return 0;
}

Uint8 rect_in_rect(Rect *a, Rect *b)
{
	if (!a || !b)
	{
		return 0;
	}

	if (a->x > b->x + b->w) return 0;
	if (b->x > a->x + a->w) return 0;
	if (a->y > b->y + b->h) return 0;
	if (b->y > a->y + a->h) return 0;
	return 1;
}

Uint8 cube_in_cube(Cube *a, Cube *b)
{
	if (!a || !b)
	{
		return 0;
	}

	if (a->x > b->x + b->w) return 0;
	if (b->x > a->x + a->w) return 0;
	if (a->y > b->y + b->h) return 0;
	if (b->y > a->y + a->h) return 0;
	if (a->z > b->z + b->d) return 0;
	if (b->z > a->z + a->d) return 0;
	return 1;
}

Uint8 point_in_sphere(float x, float y, float z, Sphere *sphere)
{
	float delta_x, delta_y, delta_z;

	if (!sphere)
	{
		return 0;
	}

	delta_x = x - sphere->x;
	delta_y = y - sphere->y;
	delta_z = z - sphere->z;

	if ((delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z) <= (sphere->r * sphere->r)) return 1;
	return 0;
}

Uint8 sphere_in_sphere(Sphere *a, Sphere *b)
{
	float delta_x, delta_y, delta_z;

	if (!a || !b)
	{
		return 0;
	}

	delta_x = a->x - b->x;
	delta_y = a->y - b->y;
	delta_z = a->z - b->z;

	if ((a->r * a->r) + (b->r + b->r) <= (delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z)) return 0;
	return 1;
}
