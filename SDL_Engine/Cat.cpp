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
	case (States::STATE_FOLLOWPLAYER):
		newState = (State*) new State_FollowPlayer;
		break;
	case (States::STATE_WANDER):
		newState = (State*) new State_Wander;
		break;
	}

	return newState;
}

int Cat::GetStateForEmotion()
{
	switch (m_currentEmotion)
	{
	case (Moods::Joy):
		return STATE_WANDER;
		break;
	case (Moods::Trust):
		return STATE_WANDER;
		break;
	case (Moods::Fear):
		return STATE_WANDER;
		break;
	case (Moods::Surprise):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Sadness):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Disgust):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Anger):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Anticipation):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Love):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Submission):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Alarm):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Disappointment):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Remorse):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Contempt):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Aggression):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Optimism):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Guilt):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Curiousity):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Pride):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Fatalism):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Delight):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Sentimentality):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Shame):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Outrage):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Pessimism):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Morbidness):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Dominance):
		return STATE_FOLLOWPLAYER;
		break;
	case (Moods::Anxiety):
		return STATE_FOLLOWPLAYER;
		break;
	default:
		return STATE_FOLLOWPLAYER;
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

	// Sort out velocity and movement
	
	if (abs(m_direction.x) > abs(m_direction.y))
	{
		if (m_direction.x < 0)
		{
			m_spriteOffset.x = 48; // left
		}
		else if (m_direction.x > 0)
		{
			m_spriteOffset.x = 32; // right
		}
	}
	else
	{
		if (m_direction.y < 0)
		{
			m_spriteOffset.x = 16; // up
		}
		else if (m_direction.y > 0)
		{
			m_spriteOffset.x = 0; // down
		}
	}

	m_velocity = Vector2(0, 0);

	m_velocity = m_direction * deltaTime * 100.0f;
	if (m_velocity.magnitude() > 100)
		m_velocity = Vector2(0, 0);

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
