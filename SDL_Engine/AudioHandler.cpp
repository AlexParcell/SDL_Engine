#include "AudioHandler.h"

std::vector<Mix_Chunk*> AudioHandler::m_soundEffects = std::vector<Mix_Chunk*>();
std::vector<Mix_Music*> AudioHandler::m_musicTracks = std::vector<Mix_Music*>();

void AudioHandler::Initialize()
{
	for (int i = 0; i < NumSoundEffects; i++)
	{
		Mix_Chunk* chunk = Mix_LoadWAV(s_soundEffectFiles[i].c_str());

		m_soundEffects.push_back(chunk);
	}

	for (int i = 0; i < NumSongs; i++)
	{
		Mix_Music* song = Mix_LoadMUS(s_songFiles[i].c_str());

		m_musicTracks.push_back(song);
	}
}

void AudioHandler::Free()
{
	for (Mix_Chunk* sfx : m_soundEffects)
	{
		Mix_FreeChunk(sfx);
	}

	for (Mix_Music* song : m_musicTracks)
	{
		Mix_FreeMusic(song);
	}
}

void AudioHandler::PlaySoundEffect(int id)
{
	if (id < m_soundEffects.size())
		Mix_PlayChannel(-1, m_soundEffects[id], 0);
}

void AudioHandler::PlaySong(int id)
{
	if (id < m_musicTracks.size())
	{
		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(m_musicTracks[id], -1);
		}
	}
}

void AudioHandler::ToggleSong()
{
	if (Mix_PlayingMusic() == 1)
	{
		if (Mix_PausedMusic() == 1)
		{
			Mix_ResumeMusic();
		}
		else
		{
			Mix_PauseMusic();
		}
	}
}

void AudioHandler::StopSong()
{
	if (Mix_PlayingMusic() == 1)
		Mix_HaltMusic();
}
