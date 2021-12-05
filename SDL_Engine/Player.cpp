#include "Player.h"
#include "Sprite.h"
#include "AudioHandler.h"
#include <iostream>

Player::Player(int type)
	: GameObject(type),
	m_moveUp(false),
	m_moveLeft(false),
	m_moveRight(false),
	m_moveDown(false)
{
	m_position = Vector2(50, 50);
	m_collisionType = CT_Block;
	m_sprite = new Sprite("me7.png");
	m_spriteSize = Vector2(16, 32);
	m_size = Vector2(32, 64);
	m_zIndex = 64;
}

Player::~Player()
{

}

SDL_Rect Player::GetBoundingBox()
{
	SDL_Rect box;
	box.x = m_position.x;
	box.y = m_position.y + (m_size.y - 4);
	box.w = m_size.x;
	box.h = 4;

	return box;
}

void Player::OnOverlap(GameObject* other)
{
	GameObject::OnOverlap(other);
}

void Player::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	m_velocity = Vector2(0, 0);
	Vector2 direction = Vector2(0, 0);
	if (m_moveUp)
	{
		direction.y = -1;
		m_spriteOffset.x = 16;
	}
	else if (m_moveDown)
	{
		direction.y = 1;
		m_spriteOffset.x = 0;
	}
	else if (m_moveLeft)
	{
		direction.x = -1;
		m_spriteOffset.x = 48;
	}
	else if (m_moveRight)
	{
		direction.x = 1;
		m_spriteOffset.x = 32;
	}

	m_velocity = direction * deltaTime * 100.0f;

	if (m_velocity.magnitude() != 0)
	{
		m_moved = true;
	}
	else
	{
		m_moved = false;
	}

	static float frameTime = 0.0f;
	const float timeBetweenFrames = 0.1f;
	frameTime += deltaTime;

	if (m_moved)
	{
		if (frameTime > timeBetweenFrames)
		{
			frameTime = 0.0f;
			m_spriteOffset.y += 32;

			if (m_spriteOffset.y > 96)
				m_spriteOffset.y = 0;
		}
	}
	else
	{
		m_spriteOffset.y = 0;
	}

	m_position += m_velocity;
}

void Player::Render()
{
	GameObject::Render();
}

void Player::OnKeyDown(SDL_Keycode key)
{
	GameObject::OnKeyDown(key);

	if (key == SDLK_1)
		AudioHandler::PlaySoundEffect(SFX_Low);

	if (key == SDLK_2)
		AudioHandler::PlaySoundEffect(SFX_Medium);

	if (key == SDLK_3)
		AudioHandler::PlaySoundEffect(SFX_High);

	if (key == SDLK_4)
		AudioHandler::PlaySoundEffect(SFX_Scratch);

	if (key == SDLK_5)
	{
		if (Mix_PlayingMusic() == 0)
		{
			AudioHandler::PlaySong(SONG_Beat);
		}
		else
		{
			AudioHandler::ToggleSong();
		}
	}

	if (key == SDLK_6)
	{
		AudioHandler::StopSong();
	}

	if (key == SDLK_w)
	{
		m_moveUp = true;
	}
	if (key == SDLK_a)
	{
		m_moveLeft = true;
	}
	if (key == SDLK_s)
	{
		m_moveDown = true;
	}
	if (key == SDLK_d)
	{
		m_moveRight = true;
	}
}

void Player::OnKeyUp(SDL_Keycode key)
{
	GameObject::OnKeyUp(key);

	if (key == SDLK_w)
	{
		m_moveUp = false;
	}
	if (key == SDLK_a)
	{
		m_moveLeft = false;
	}
	if (key == SDLK_s)
	{
		m_moveDown = false;
	}
	if (key == SDLK_d)
	{
		m_moveRight = false;
	}
}
