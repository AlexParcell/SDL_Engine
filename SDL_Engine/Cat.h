#pragma once

#include "GameObject.h"
#include "EmotionUtilities.h"
#include "State.h"

class Text;

class Cat : public GameObject
{
	float m_frameTimer;
	float m_timeSinceLastMeow;
	State* m_state;

	Sprite* m_exclamationBox;
	Sprite* m_fireBox;
	Sprite* m_heartBox;
	Text* m_nametag;

public:
	Vector2 m_direction;
	EmotionalState m_emotionalState;
	EmotionalState m_baselineEmotionalState;
	Personality m_personality;
	std::string m_name;
	int m_currentEmotion;
	int m_lastEmotion;
	float m_timeSinceLastGrowl;
	float m_timeSinceLastPurr;

	Cat(int type, std::string name, Personality personality);
	~Cat();

	void RecieveEmotionalEvent(EmotionalEvent* e);

	virtual SDL_Rect GetBoundingBox();
	virtual void OnOverlap(GameObject* other);
	void Meow();
	void Growl();
	void Purr();
	State* MakeNewState(int type);
	int GetStateForEmotion();
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void OnKeyDown(SDL_Keycode key);
	virtual void OnKeyUp(SDL_Keycode key);
};