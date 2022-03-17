#pragma once

class Cat;
class Vector2;
#include <vector>

enum States
{
	STATE_NONE,
	STATE_FOLLOWPLAYER,
	STATE_WANDER,
	STATE_PURR,
	STATE_HIDE,
	STATE_GROWL
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

class State_Purr : public State
{
public:
	int m_type;
	State_Purr() : m_type(STATE_PURR) {}
	~State_Purr() {}

	virtual void Enter(Cat* cat);
	virtual void Exit(Cat* cat);
	virtual void Update(float dt, Cat* cat);
};

class State_Hide : public State
{
public:
	int m_type;
	State_Hide() : m_type(STATE_HIDE) {}
	~State_Hide() {}

	virtual void Enter(Cat* cat);
	virtual void Exit(Cat* cat);
	virtual void Update(float dt, Cat* cat);
};

class State_Growl : public State
{
	Cat* m_target;
	Cat* m_lastTarget;
public:
	int m_type;
	State_Growl() : m_type(STATE_GROWL), m_target(nullptr), m_lastTarget(nullptr) {}
	~State_Growl() {}

	virtual void Enter(Cat* cat);
	virtual void Exit(Cat* cat);
	virtual void Update(float dt, Cat* cat);
};