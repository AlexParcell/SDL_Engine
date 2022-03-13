#include "Sprite.h"
#include "AudioHandler.h"
#include <iostream>

#include <stdlib.h> 
#include <time.h> 

#include "Cat.h"

Cat::Cat(int type, std::string name, Personality personality)
	: GameObject(type),
	m_direction(0, 0),
	m_frameTimer(0.0f),
	m_moveTimer(0.0f),
	m_meowTimer(0.0f),
	m_timeSinceLastMeow(3.0f),
	m_exclamationBox(nullptr),
	m_name(name),
	m_personality(personality)
{
	srand(time(NULL));
	m_collisionType = CT_Block;
	m_sprite = new Sprite("cat.png");
	m_exclamationBox = new Sprite("exclamation.png");
	m_spriteSize = Vector2(16, 16);
	m_size = Vector2(32, 32);
	m_zIndex = 32;
	m_baselineEmotionalState = GetBaselineEmotionalState(m_personality);
	m_emotionalState = m_baselineEmotionalState;
}

Cat::~Cat()
{
	delete m_exclamationBox;
}

SDL_Rect Cat::GetBoundingBox()
{
	SDL_Rect box;
	box.x = m_position.x;
	box.y = m_position.y + (m_size.y - 4);
	box.w = m_size.x;
	box.h = 4;

	return box;
}

void Cat::OnOverlap(GameObject* other)
{
	GameObject::OnOverlap(other);

	if (other->m_collisionType == CT_Block)
	{
		m_direction *= -1;
	}
}

void Cat::Meow()
{
	m_timeSinceLastMeow = 0.0f;
	AudioHandler::PlaySoundEffect(SFX_Meow);
}

void Cat::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	// Do we wanna meow?
	m_timeSinceLastMeow += deltaTime;
	static float meowInterval = 2.0f;
	m_meowTimer += deltaTime;
	if (m_meowTimer > meowInterval)
	{
		m_meowTimer = 0.0f;
		if (rand() % 10 == 0)
			Meow();
	}

	// Pick a movement direction
	const float directionInterval = 2.0f;
	m_moveTimer += deltaTime;

	if (m_moveTimer > directionInterval)
	{
		m_moveTimer = 0.0f;

		int direction = rand() % 4;

		switch (direction)
		{
		case (0):
			m_direction = Vector2(-1, 0);
			break;
		case (1):
			m_direction = Vector2(1, 0);
			break;
		case (2):
			m_direction = Vector2(0, 1);
			break;
		case (3):
			m_direction = Vector2(0, -1);
			break;
		}
	}


	// Sort out velocity and movemen
	
	if (m_direction.y == -1)
	{
		m_spriteOffset.x = 16;
	}
	else if (m_direction.y == 1)
	{
		m_spriteOffset.x = 0;
	}
	else if (m_direction.x == -1)
	{
		m_spriteOffset.x = 48;
	}
	else if (m_direction.x == 1)
	{
		m_spriteOffset.x = 32;
	}
	m_velocity = Vector2(0, 0);

	m_velocity = m_direction * deltaTime * 100.0f;

	if (m_velocity.magnitude() != 0)
	{
		m_moved = true;
	}
	else
	{
		m_moved = false;
	}

	const float timeBetweenFrames = 0.1f;
	m_frameTimer += deltaTime;

	if (m_moved)
	{
		if (m_frameTimer > timeBetweenFrames)
		{
			m_frameTimer = 0.0f;
			m_spriteOffset.y += 16;

			if (m_spriteOffset.y > 48)
				m_spriteOffset.y = 0;
		}
	}
	else
	{
		m_spriteOffset.y = 0;
	}

	m_position += m_velocity;
}

void Cat::Render()
{
	GameObject::Render();

	if (m_timeSinceLastMeow < 1.0f)
	{
		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = 16;
		src.h = 16;
		SDL_Rect dest;
		dest.x = m_position.x;
		dest.y = m_position.y - 32;
		dest.w = 32;
		dest.h = 32;
		m_exclamationBox->Render(&src, &dest);
	}
}

void Cat::OnKeyDown(SDL_Keycode key)
{
	GameObject::OnKeyDown(key);
}

void Cat::OnKeyUp(SDL_Keycode key)
{
	GameObject::OnKeyUp(key);
}
