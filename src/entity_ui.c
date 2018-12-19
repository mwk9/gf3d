#include "entity_ui.h"
#include "ui.h"
#include "gf3d_vgraphics.h"

UIPipeline *uiPipeline;

SDL_Surface *surface;
SDL_Renderer *renderer;
int lastPosition = 0;
Texture *interfaceTexture;

SDL_Surface *entity_ui_sdl_surface_init(Uint32 width, Uint32 height)
{
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	return SDL_CreateRGBSurface
	(
		0,
		width, height,
		32,
		rmask, gmask, bmask, amask
	);
}

void entity_ui_sdl_render_screen_clear()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void entity_ui_sdl_create()
{
	surface = entity_ui_sdl_surface_init(gf3d_vgraphics_get_view_extent().width, gf3d_vgraphics_get_view_extent().height);
	renderer = SDL_CreateSoftwareRenderer(surface);
	interfaceTexture = gf3d_texture_surface_create(surface);
}

void entity_ui_sdl_draw()
{
	SDL_Rect rect;

	entity_ui_sdl_render_screen_clear();

	rect.x = lastPosition;
	rect.y = 50;
	rect.w = 50;
	rect.h = 50;
	SDL_SetRenderDrawColor(renderer, 121, 52, 192, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}

void entity_ui_update(Entity *self)
{
	static int lastUpdate = 10;
	if (lastPosition > surface->w)
	{
		lastPosition = 0;
	}
	lastPosition++;
	if (lastUpdate >= 10)
	{
		entity_ui_sdl_draw();
		gf3d_texture_surface_update(interfaceTexture, surface);
		lastUpdate = 0;
	}
	lastUpdate++;
}

void entity_ui_draw(Entity *self)
{
	ui_pipeline_renderpass(uiPipeline, self->commandBuffer, self->bufferFrame);
}

Entity *entity_ui_init()
{
	Entity *e;

	entity_ui_sdl_create();
}
