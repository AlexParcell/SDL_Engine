#pragma once

class Cat;

enum States
{
	STATE_NONE,
	STATE_DEFAULT
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

class DefaultState
{
public:
	int m_type;
	DefaultState() : m_type(STATE_DEFAULT) {}
	~DefaultState() {}

	virtual void Enter(Cat* cat);
	virtual void Exit(Cat* cat);
	virtual void Update(float dt, Cat* cat);
};