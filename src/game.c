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
	Entity *test_ent;
	Entity *test_ent2 = NULL;
	float direction = 0.0f;
	Sound *dootdoot = NULL;
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
	entity_system_init(MAX_ENTITY_NUM);
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
	test_ent = entity_load("bird_maya2");
	test_ent->position.x = 5.0f;
	test_ent->position.z = -5.0f;

	//test_ent2 = entity_load("agumon");
	//test_ent2->position.x = -1.0f;
	//test_ent2->position.y = -3.0f;
	
	//test_ent2->ubo->model[3][0] = 5;
	//test_ent->rotation = vector3d(0.0f, 1.0f, 0.0f);
	//test_ent2 = entity_load("cube");

	dootdoot = sound_load("audio/rift.ogg", 5.0f, -1);
	sound_play(dootdoot, -1, 0, -1, 0);

    while(!done)
    {
		test_ent->rotation.z = 0.05f;
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

		if (keys[SDL_SCANCODE_B])
		{
			test_ent2 = entity_load("agumon");
			//entity_set_draw_position(test_ent, vector3d(0.0f,0.0f,0.0f));
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
