#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>

enum SoundEffects
{
	SFX_Meow,
	SFX_Spray,
	SFX_Purr,
	SFX_Growl,
	SFX_Vacuum,
	SFX_Toy,
	NumSoundEffects
};

static std::string s_soundEffectFiles[NumSoundEffects] =
{
	"Audio/meow.wav",
	"Audio/Spray.wav",
	"Audio/Purr.wav",
	"Audio/growl.wav",
	"Audio/vacuum.wav",
	"Audio/toy.wav"
};

enum Songs
{
	SONG_Beat,
	NumSongs
};

static std::string s_songFiles[NumSongs] =
{
	"Audio/beat.wav"
};

class AudioHandler
{
	static std::vector<Mix_Chunk*> m_soundEffects;
	static std::vector<Mix_Music*> m_musicTracks;

public:
	static void Initialize();
	static void Free();
	static void PlaySoundEffect(int id);

	static void PlaySong(int id);
	static void ToggleSong();
	static void StopSong();
};