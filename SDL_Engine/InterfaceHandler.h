#pragma once

class HeadsUpDisplay
{
public:
	HeadsUpDisplay();
	~HeadsUpDisplay();

	void Update(float deltaTime);
	void Render();
};

class InterfaceHandler
{
	static HeadsUpDisplay* m_hud;
public:

	static void OpenHUD();
	static void Update(float deltaTime);
	static void Render();
	static void Free() { if (m_hud) delete m_hud; }

	static HeadsUpDisplay* GetHUD() { return m_hud; }

};