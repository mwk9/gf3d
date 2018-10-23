#include "shape.h"

Rect * rect_new(float x, float y, float width, float height)
{
	Rect *r = NULL;
	r = (Rect *)malloc(sizeof(Rect));
	if (!r)
	{
		slog("Error: could not allocate space for a new Rect");
		return NULL;
	}
	memset(r, 0, sizeof(Rect));
	r->x = x;
	r->y = y;
	r->w = width;
	r->h = height;
	return r;
}

Cube * cube_new(float x, float y, float z, float width, float height, float depth)
{
	Cube *c = NULL;
	c = (Cube *)malloc(sizeof(Cube));
	if (!c)
	{
		slog("Error: could not allocate space for a new Cube");
		return NULL;
	}
	memset(c, 0, sizeof(Cube));
	c->x = x;
	c->y = y;
	c->z = z;
	c->w = width;
	c->h = height;
	c->d = depth;
	return c;
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
