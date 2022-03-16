#include "Sprite.h"
#include "AudioHandler.h"
#include <iostream>

#include <stdlib.h> 
#include <time.h> 

#include "Cat.h"
#include "Text.h"

Cat::Cat(int type, std::string name, Personality personality)
	: GameObject(type),
	m_direction(0, 0),
	m_frameTimer(0.0f),
	m_moveTimer(0.0f),
	m_timeSinceLastMeow(3.0f),
	m_exclamationBox(nullptr),
	m_name(name),
	m_personality(personality),
	m_currentEmotion(0),
	m_lastEmotion(0),
	m_nametag(nullptr)
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
	EmotionalEventHandler::m_bus->subscribe(this, &Cat::RecieveEmotionalEvent);
	m_nametag = new Text(m_name, { 255, 255, 255 }, 16);
}

Cat::~Cat()
{
	delete m_state;
	delete m_exclamationBox;
	delete m_nametag;
}

void Cat::RecieveEmotionalEvent(EmotionalEvent* e)
{
	EmotionalState adjustedImpulse = AdjustEmotionalImpulse(e->m_impulse, m_personality);
	std::cout << "Applying impulse of " << adjustedImpulse[0] << ", " << adjustedImpulse[1] << ", " << adjustedImpulse[2] << ", " << adjustedImpulse[3] << " to " << m_name << std::endl;
 	EmotionalState newEmotionalState = m_emotionalState + adjustedImpulse;

	for (float& v : newEmotionalState.m_emotionalAxis)
	{
		v = clip(v, -1, 1);
	}

	m_emotionalState = newEmotionalState;
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

State* Cat::MakeNewState(int type)
{
	State* newState = nullptr;

	switch (type)
	{
	case (States::STATE_DEFAULT):
		newState = (State*) new DefaultState;
		break;
	}

	return newState;
}

int Cat::GetStateForEmotion()
{
	switch (m_currentEmotion)
	{
	case (Moods::Joy):
	case (Moods::Trust):
	case (Moods::Fear):
	case (Moods::Surprise):
	case (Moods::Sadness):
	case (Moods::Disgust):
	case (Moods::Anger):
	case (Moods::Anticipation):
	case (Moods::Love):
	case (Moods::Submission):
	case (Moods::Alarm):
	case (Moods::Disappointment):
	case (Moods::Remorse):
	case (Moods::Contempt):
	case (Moods::Aggression):
	case (Moods::Optimism):
	case (Moods::Guilt):
	case (Moods::Curiousity):
	case (Moods::Pride):
	case (Moods::Fatalism):
	case (Moods::Delight):
	case (Moods::Sentimentality):
	case (Moods::Shame):
	case (Moods::Outrage):
	case (Moods::Pessimism):
	case (Moods::Morbidness):
	case (Moods::Dominance):
	case (Moods::Anxiety):
	default:
		return STATE_DEFAULT;
		break;
	}
}

void Cat::Update(float deltaTime)
{
	GameObject::Update(deltaTime);


	// If we got no state, better sort that out
	if (m_state == nullptr)
	{
		int stateToBeOn = GetStateForEmotion();
		m_state = MakeNewState(stateToBeOn);
		m_state->Enter(this);
	}

	m_emotionalState = lerp(m_emotionalState, m_baselineEmotionalState, deltaTime * 0.01);
	m_currentEmotion = IdentifyEmotionalState(m_emotionalState);
	if (m_currentEmotion != m_lastEmotion)
	{
		// Signal the new emotion
		Meow();

		// Pick a new state if need be
		int stateToBeOn = GetStateForEmotion();
		if (stateToBeOn != m_state->m_type)
		{
			m_state->Exit(this);
			delete m_state;

			m_state = MakeNewState(stateToBeOn);
			m_state->Enter(this);
		}

		std::cout << m_name << "'s emotional state changed from " << g_sMoodValues[m_lastEmotion] << " to " << g_sMoodValues[m_currentEmotion] << std::endl;
	}
	m_timeSinceLastMeow += deltaTime;

	// Update our state
	m_state->Update(deltaTime, this);

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

	// Sort out velocity and movement
	
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
	m_lastEmotion = m_currentEmotion;
}

void Cat::Render()
{
	GameObject::Render();

	m_nametag->m_position = Vector2((m_position.x + (m_size.x / 2)) - m_nametag->m_size.x / 2, m_position.y - 16);
	m_nametag->Render();

	if (m_timeSinceLastMeow < 1.0f)
	{
		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = 16;
		src.h = 16;
		SDL_Rect dest;
		dest.x = m_position.x;
		dest.y = m_position.y - 48;
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
