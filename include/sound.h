#ifndef __SOUND__
#define __SOUND__

#define FILEPATH_CHAR_LEN 512			/**<Maximum length for a filepath*/
#define MAX_SOUND_NUM 64
#define MAX_CHANNEL_NUM 4

#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>
#include "simple_logger.h"

/**
 * @brief A structure representing a chunk of audio
 */
typedef struct sound_s
{
	Uint64 id;							/**<Internal counter*/
	Uint8 inUse;						/**<If the Sound is currently being used in memory*/
	char filepath[FILEPATH_CHAR_LEN];	/**<The location of the audio file*/
	Mix_Chunk *sound;					/**<The actual audio chunk*/
	float volume;						/**<The Sound's volume*/
	int defaultChannel;					/**<Default channel to play the sound on*/
}Sound;

/**
 * @brief Initializes the audio system, will also initialize the sound system if it is not currently initialized
 * @param maxSounds The maximum number of sounds that can exist in memory
 * @param channels The number of channels to allocate
 * @param channelGroups Currently unused
 * @param maxMusic Currently unused
 * @param enableMP3 Enables loading of .mp3 files if parameter is not 0
 * @param enableOGG Enables loading of .ogg files is parameter is not 0
 */
void audio_system_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8 enableMP3, Uint8 enableOGG);

/**
 * @brief Initializes the sound system, called by audio_system_init if sound system is not currently initialized
 * @param maxSounds The maximum number of sounds that can exist in memory
 */
void sound_system_init(Uint32 maxSounds);

/**
 * @brief Returns an unused space in memory for a new Sound
 * @returns A pointer to a Sound on success; NULL if no more space available
 */
Sound *sound_new();

/**
 * @brief Checks to see if a specified Sound is already loaded into memory
 * @returns A pointer to the specified Sound if it is already loaded into memory; NULL on failure to find the specified Sound
 */
Sound *sound_get_by_filepath(char *filepath);

/**
 * @brief Creates a new Sound from a specified file
 * @param filepath The location of the audio file to load
 * @param volume The starting volume for the audio
 * @param defaultChannel The default channel to play the audio from, -1 to automatically find an unused channel
 8 @returns A pointer to the loaded sound on success; NULL if failed to load a new sound
 */
Sound *sound_load(char *filepath, float volume, int defaultChannel);

/**
 * @brief Plays a specified Sound
 * @param sound The Sound structure to play
 * @param numLoops The number of times to play the specified sound, -1 to loop infinately
 * @param volume The volume of the audio file, 0 or less to use the default audio level
 * @param channel The channel to play the audio on, -1 to find a channel automatically
 * @param group Currently unused
 */
void sound_play(Sound *sound, int numLoops, float volume, int channel, int group);

/**
 * @brief Adjusts the volume of a Sound
 * @param sound The Sound structure to alter
 * @param volume The new volume amount for the specified Sound
 */
void sound_adjust_volume(Sound *sound, float volume);

/**
 * @brief Uniformly adjusts the volume for all loaded Sounds
 * @param volume The new volume for each Sound
 */
void sound_adjust_volume_all(float volume);

/**
 * @brief Marks a Sound as free, letting another Sound use its place in memory
 * @param sound The Sound to free
 */
void sound_free(Sound *sound);

/**
 * @brief Zeroes out a section of memory where a Sound is stored
 * @param sound The Sound to delete from memory
 */
void sound_delete(Sound *sound);

/**
 * @brief Zeroes out all Sounds loaded in memory
 */
void sound_delete_all();

#endif //!__SOUND__
