#include "sound.h"

typedef struct soundManager_s
{
	Uint32 maxSounds;
	Sound *soundList;
	Uint64 increment;
	Uint8 initialized;
}SoundManager;

static SoundManager soundManager = { 0, NULL, 0, 0 };

/**
 * @brief Closes the audio and sound systems
 */
void audio_system_close()
{
	int i = 0;

	if (soundManager.soundList != NULL)
	{
		for (i = 0; i < soundManager.maxSounds; i++)
		{
			sound_free(&soundManager.soundList[i]);
		}
		free(soundManager.soundList);
	}
	memset(&soundManager, 0, sizeof(SoundManager));
	slog("Audio system closed.");
}

void audio_system_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8 enableMP3, Uint8 enableOGG)
{
	int flags = 0;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		slog("Error: Failed to initialize audio system: %s", SDL_GetError());
		return;
	}

	if (enableMP3)
	{
		flags |= MIX_INIT_MP3;
	}
	if (enableOGG)
	{
		flags |= MIX_INIT_OGG;
	}
	if (!(Mix_Init(flags) & flags))
	{
		slog("Error: Failed to initialize some audio support: %s", SDL_GetError());
	}
	Mix_AllocateChannels(channels);

	if (soundManager.initialized == 0)
	{
		sound_system_init(maxSounds);
	}

	atexit(Mix_Quit);
	atexit(audio_system_close);
}

void sound_system_init(Uint32 maxSounds)
{
	if (maxSounds <= 0)
	{
		slog("Error: Cannot initialize a sound manager for zero or less sounds.");
		return;
	}

	memset(&soundManager, 0, sizeof(SoundManager));
	soundManager.soundList = (Sound *)malloc(sizeof(Sound)* maxSounds);
	if (!soundManager.soundList)
	{
		slog("Error: Could not allocate memory for the sound list.");
		audio_system_close();
		return;
	}
	memset(soundManager.soundList, 0, sizeof(Sound)* maxSounds);
	soundManager.maxSounds = maxSounds;
	soundManager.initialized = 1;

	slog("Sound system initialized.");
	atexit(audio_system_close);
}

Sound *sound_new()
{
	int i = 0;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		if (soundManager.soundList[i].inUse == 0)
		{
			slog("Found a spot for a sound at index (%i)", i);
			memset(&soundManager.soundList[i], 0, sizeof(Sound));
			soundManager.soundList[i].id = soundManager.increment++;
			soundManager.soundList[i].inUse = 1;
			return &soundManager.soundList[i];
		}
	}

	slog("Error: Out of spaces for a new sound.");
	return NULL;
}

Sound *sound_get_by_filepath(char *filepath)
{
	int i = 0;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		if (strncmp(soundManager.soundList[i].filepath, filepath, FILEPATH_CHAR_LEN) == 0)
		{
			return &soundManager.soundList[i];
		}
	}

	slog("Warning: Could not find sound with filepath (%s)", filepath);
	return NULL;
}

Sound *sound_load(char *filepath, float volume, int defaultChannel)
{
	Sound *sound;

	if (!filepath)
	{
		slog("Error: Cannot load a sound from an empty string filepath");
		return NULL;
	}

	sound = sound_get_by_filepath(filepath);
	if (sound)
	{
		sound->inUse = 1;
		return sound;
	}

	sound = sound_new();
	sound->sound = Mix_LoadWAV(filepath);
	if (!sound->sound)
	{
		slog("Error: Failed to load sound file (%s), error (%s)", filepath, Mix_GetError());
		sound_free(sound);
		return NULL;
	}

	sound->volume = volume;
	sound->defaultChannel = defaultChannel;
	strncpy(sound->filepath, filepath, FILEPATH_CHAR_LEN);
	return sound;
}

void sound_play(Sound *sound, int numLoops, float volume, int channel, int group)
{
	//int _channel;
	float _volume = 1.0f;
	if (!sound)
	{
		slog("Error: Cannot play a sound that is NULL.");
		return;
	}

	if (volume > 0)
	{
		_volume *= volume;
		sound->volume = _volume;
	}
	//_channel = sound->defaultChannel;
	Mix_VolumeChunk(sound->sound, (int)sound->volume);
	Mix_PlayChannel(sound->defaultChannel, sound->sound, numLoops);
}

void sound_play_get_by_filepath(char *filepath, int numLoops, float volume, int channel, int group)
{
	int i = 0;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		if (strncmp(soundManager.soundList[i].filepath, filepath, FILEPATH_CHAR_LEN) == 0)
		{
			sound_play(&soundManager.soundList[i], numLoops, volume, channel, group);
			return;
		}
	}

	slog("Warning: Could not find sound with filepath (%s)", filepath);
	return;
}

void sound_adjust_volume(Sound *sound, float volume)
{
	if (!sound)
	{
		slog("Error: Cannot adjust volume of a sound that is NULL.");
		return;
	}

	Mix_VolumeChunk(sound->sound, volume);
}

void sound_adjust_volume_all(float volume)
{
	int i = 0;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		if (soundManager.soundList[i].inUse > 0)
		{
			sound_adjust_volume(&soundManager.soundList[i], volume);
		}
	}
}

void sound_free(Sound *sound)
{
	if (!sound)
	{
		slog("Error: Cannot free a NULL sound.");
		return;
	}

	sound->inUse = 0;
}

void sound_delete(Sound *sound)
{
	if (!sound)
	{
		slog("Error: Cannot delete a NULL sound.");
		return;
	}

	if (sound->sound != NULL)
	{
		Mix_FreeChunk(sound->sound);
	}
	memset(sound, 0, sizeof(Sound));
}

void sound_delete_all()
{
	int i = 0;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		sound_delete(&soundManager.soundList[i]);
	}
}
