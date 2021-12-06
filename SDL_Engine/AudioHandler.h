#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>

enum SoundEffects
{
	SFX_Low,
	SFX_Medium,
	SFX_High,
	SFX_Scratch,
	SFX_Meow,
	NumSoundEffects
};

static std::string s_soundEffectFiles[NumSoundEffects] =
{
	"Audio/low.wav",
	"Audio/medium.wav",
	"Audio/high.wav",
	"Audio/scratch.wav",
	"Audio/meow.wav"
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