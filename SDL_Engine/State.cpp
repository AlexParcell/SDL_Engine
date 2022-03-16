#include "State.h"
#include "Pathfinder.h"
#include <iostream>

void DefaultState::Enter(Cat* cat)
{
	std::cout << "Entering default state" << std::endl;
}

void DefaultState::Exit(Cat* cat)
{
	std::cout << "Exiting default state" << std::endl;
}

void DefaultState::Update(float dt, Cat* cat)
{
	Pathfinder::GetPath(Vector2(32, 96), Vector2(128, 96));
}
