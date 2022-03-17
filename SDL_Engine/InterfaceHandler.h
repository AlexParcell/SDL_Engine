#pragma once

#include <vector>
#include <string>
#include "Sprite.h"
#include "Vector2.h"

class Cat;

enum Tools
{
	Tool_None,
	Tool_PetCat,
	Tool_Vacuum,
	Tool_Toy,
	Tool_SprayBottle,
	Tool_Cucumber,
	Tool_LaserPointer,
	Tool_Mouse,
	Tool_Brush,
	NumTools
};

static std::vector<std::string> s_Tools = {
	"None",
	"Petting Hand",
	"Vacuum Cleaner",
	"Bell Toy",
	"Spray Bottle",
	"Cucumber",
	"Laser Pointer",
	"Mouse Toy",
	"Brush",
};

class HeadsUpDisplay
{
	int m_equippedTool;
	Sprite* m_petSprite;
	Sprite* m_vacuumSprite;
	Sprite* m_toySprite;
	Sprite* m_sprayBottleSprite;
	Sprite* m_cucumberSprite;
	Sprite* m_mouseSprite;
	Sprite* m_brushSprite;
	Sprite* m_laserPointerSprite;

	float m_timeSinceLastMouseClick;
	Vector2 m_mousePositionLastFrame;

	int m_vacuumFrame;
	int m_toyFrame;
	bool m_mouseButtonDown;
	std::vector<std::string> m_logEntries;
public:
	HeadsUpDisplay();
	~HeadsUpDisplay();

	void Update(float deltaTime);
	void Render();
	std::vector<Cat*> GetAllCatsWithinRadius(Vector2 point, float distance);
	void MouseButtonDown();
	void MouseButtonUp();
	void PushLogEntry(std::string entry);
};

class InterfaceHandler
{
	static HeadsUpDisplay* m_hud;
public:

	static void OpenHUD();
	static void Update(float deltaTime);
	static void Render();
	static void Free() { if (m_hud) delete m_hud; }
	static void MouseButtonDown() { if (m_hud) m_hud->MouseButtonDown(); }
	static void MouseButtonUp() { if (m_hud) m_hud->MouseButtonUp(); }
	static void PushLogEntry(std::string entry) { if (m_hud) m_hud->PushLogEntry(entry); }

	static HeadsUpDisplay* GetHUD() { return m_hud; }

};