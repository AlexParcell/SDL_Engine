#pragma once

#include "GameObject.h"
#include "EmotionUtilities.h"

class Cat : public GameObject
{
	Vector2 m_direction;

	float m_frameTimer;
	float m_moveTimer;

	float m_meowTimer;
	float m_timeSinceLastMeow;

	Sprite* m_exclamationBox;

public:
	EmotionalState m_emotionalState;
	EmotionalState m_baselineEmotionalState;
	Personality m_personality;
	std::string m_name;

	Cat(int type, std::string name, Personality personality);
	~Cat();

	virtual SDL_Rect GetBoundingBox();
	virtual void OnOverlap(GameObject* other);
	void Meow();
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void OnKeyDown(SDL_Keycode key);
	virtual void OnKeyUp(SDL_Keycode key);
};