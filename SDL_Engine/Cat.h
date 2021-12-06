#pragma once

#include "GameObject.h"

class Cat : public GameObject
{

	bool m_moveUp;
	bool m_moveDown;
	bool m_moveLeft;
	bool m_moveRight;

	float m_frameTimer;
	float m_moveTimer;

	float m_meowTimer;
	float m_timeSinceLastMeow;

	Sprite* m_exclamationBox;

public:
	Cat(int type);
	~Cat();

	virtual SDL_Rect GetBoundingBox();
	virtual void OnOverlap(GameObject* other);
	void Meow();
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void OnKeyDown(SDL_Keycode key);
	virtual void OnKeyUp(SDL_Keycode key);
};