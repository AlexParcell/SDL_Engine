#pragma once

class Cat;
class Vector2;
#include <vector>

enum States
{
	STATE_NONE,
	STATE_FOLLOWPLAYER,
	STATE_WANDER
};

class State
{
public:
	int m_type;
	State() : m_type(STATE_NONE) {}
	~State() {}

	virtual void Enter(Cat* cat) = 0;
	virtual void Exit(Cat* cat) = 0;
	virtual void Update(float dt, Cat* cat) = 0;
};

class State_FollowPlayer : public State
{
	std::vector<Vector2> m_path;
public:
	int m_type;
	State_FollowPlayer() : m_type(STATE_FOLLOWPLAYER) {}
	~State_FollowPlayer() {}

	virtual void Enter(Cat* cat);
	virtual void Exit(Cat* cat);
	virtual void Update(float dt, Cat* cat);
};

class State_Wander : public State
{
	float m_moveTimer;
public:
	int m_type;
	State_Wander() : m_type(STATE_WANDER), m_moveTimer(0.0f) {}
	~State_Wander() {}

	virtual void Enter(Cat* cat);
	virtual void Exit(Cat* cat);
	virtual void Update(float dt, Cat* cat);
};