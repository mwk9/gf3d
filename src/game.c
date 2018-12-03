#include <SDL.h>            

#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_matrix.h"
#include "gf3d_camera.h"
#include "gf3d_vector.h"
#include "gf3d_texture.h"
#include "entity.h"
#include "player.h"
//#include "shape.h"
#include "uniforms.h"
#include "sound.h"

int main(int argc,char *argv[])
{
    int done = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    //Model *model;
    //Model *model2;
	Entity *test_ent = NULL;
	Entity *test_ent2 = NULL;
	Entity *floor = NULL;
	float direction = 0.0f;
	Sound *dootdoot = NULL;
	Shape *s1 = NULL;
	Shape *s2 = NULL;
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
	entity_system_init(MAX_ENTITY_NUM);
	shape_system_init(MAX_ENTITY_NUM * 2);
	audio_system_init(MAX_SOUND_NUM, MAX_CHANNEL_NUM, 0, 0, 1, 1);
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        1                      //validation
    );
    
    // main game loop
    slog("gf3d main loop begin");
    //model = gf3d_model_load("bird_maya");
    //model2 = gf3d_model_load("cube");
	//test_ent = entity_load("pigeon", -1);
	//test_ent = entity_new();
	test_ent = entity_player_init(test_ent, NULL);
	entity_load_from_file("def/test.txt");
	//test_ent->position.x = 0.0f;
	//test_ent->position.z = 0.0f;
	//test_ent->useGravity = 1;
	//entity_scale(test_ent, vector3d(0.1f, 0.1f, 0.1f));
	//test_ent->shape = cube_new(test_ent->position.x, test_ent->position.y, test_ent->position.z, 3.0f, 3.0f, 3.0f);
	//test_ent->rotation.x = 1.0f;
	//gf3d_matrix_rotate(test_ent->ubo->model, test_ent->ubo->model, test_ent->rotation.x, vector3d(1, 0, 0));
	//test_ent->rotation.x = 0.0f;

	floor = entity_load("grass", 100);
	floor->isStatic = 1;
	floor->position.z = -10.0f;
	entity_scale(floor, vector3d(100.0f, 100.0f, 4.0f));
	floor->shape = cube_new(floor->position.x - 50.0f, floor->position.y - 50.0f, floor->position.z + 1.5f, 100.0f, 100.0f, 3.0f);

	//test_ent2 = entity_load("agumon");
	//test_ent2->position.x = -1.0f;
	//test_ent2->position.y = -3.0f;
	
	//test_ent2->ubo->model[3][0] = 5;
	//test_ent->rotation = vector3d(0.0f, 1.0f, 0.0f);
	//test_ent2 = entity_load("cube");

	dootdoot = sound_load("audio/rift.ogg", 15.0f, -1);
	sound_play(dootdoot, -1, 0, -1, 0);

    while(!done)
    {
		if (test_ent)
		{
			//test_ent->position.y -= 1;
			//test_ent->rotation.z = 0.05f;
		}
		if (test_ent2)
		{
			//test_ent2->rotation.x = 0.1f;
			test_ent2->position.x -= 0.1f;
			test_ent2->rotation.y = 0.075f;
		}
		//test_ent2->rotation.y -= 0.05f;
		//test_ent2->rotation.z -= 0.1f;
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
        
        //gf3d_vgraphics_rotate_camera(0.03);
        
        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        commandBuffer = gf3d_command_rendering_begin(bufferFrame);

            //gf3d_model_draw(model,bufferFrame,commandBuffer);
            //gf3d_model_draw(model2,bufferFrame,commandBuffer);
		entity_update_all();

		entity_draw_all(bufferFrame, commandBuffer);
		//entity_draw(test_ent, bufferFrame, commandBuffer);
            
        
		if (keys[SDL_SCANCODE_D])
		{
			direction = -1.0f;
		}
		else if (keys[SDL_SCANCODE_A])
		{
			direction = 1.0f;
		}
		else
		{
			direction = 0.0f;
		}
		gf3d_command_rendering_end(commandBuffer);
		gf3d_vgraphics_render_end(bufferFrame, keys, direction);
		
		/*if (test_ent && floor)
		{
			if (cube_in_cube(&test_ent->shape->shape.cube, &floor->shape->shape.cube))
			{
				//slog("collision!");
				test_ent->acceleration = vector3d(0.0f, 0.0f, 0.0f);
				test_ent->useGravity = 0;
			}
			else
			{
				test_ent->useGravity = 1;
			}
		}
		if (test_ent2 && floor)
		{
			if (cube_in_cube(&test_ent2->shape->shape.cube, &floor->shape->shape.cube))
			{
				//slog("collision!");
				test_ent2->acceleration = vector3d(0.0f, 0.0f, 0.0f);
				test_ent2->useGravity = 0;
			}
			else
			{
				test_ent2->useGravity = 1;
			}
		}*/
		TEST_all_entities_collide_with_floor(floor);

		if (keys[SDL_SCANCODE_B])
		{
			test_ent2 = entity_load("agumon", -1);
			test_ent2->useGravity = 1;
			test_ent2->shape = cube_new(test_ent2->position.x, test_ent2->position.y, test_ent2->position.z, 3.0f, 3.0f, 3.0f);
			//entity_set_draw_position(test_ent, vector3d(0.0f,0.0f,0.0f));
			entity_load("cube", -1)->velocity.x = rand() % 7;
		}
		if (keys[SDL_SCANCODE_N])
		{
			entity_free(test_ent2);
		}

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
