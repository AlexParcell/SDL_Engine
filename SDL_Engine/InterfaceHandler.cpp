#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_sdlrenderer.h"
#include "InterfaceHandler.h"
#include <SDL.h>
#include "Cat.h"
#include "LevelHandler.h"
#include "Level.h"
#include "EmotionUtilities.h"
#include "Sprite.h"
#include "InputHandler.h"
#include "AudioHandler.h"
#include <iostream>

HeadsUpDisplay* InterfaceHandler::m_hud = nullptr;

extern SDL_Window* g_window;

void InterfaceHandler::OpenHUD()
{
	if (m_hud)
		delete m_hud;

	m_hud = new HeadsUpDisplay();
}

void InterfaceHandler::Update(float deltaTime)
{
	if (m_hud)
		m_hud->Update(deltaTime);
}

void InterfaceHandler::Render()
{
	if (m_hud)
		m_hud->Render();
}

HeadsUpDisplay::HeadsUpDisplay()
	: m_equippedTool(0),
	m_timeSinceLastMouseClick(100.0f),
	m_mousePositionLastFrame(Vector2(0, 0)),
	m_vacuumFrame(0),
	m_toyFrame(0),
	m_mouseButtonDown(false)
{
	m_petSprite = new Sprite("Hand.png");
	m_vacuumSprite = new Sprite("Vacuum.png");
	m_sprayBottleSprite = new Sprite("SprayBottle.png");
	m_toySprite = new Sprite("Toy.png");
	m_cucumberSprite = new Sprite("Cucumber.png");
	m_laserPointerSprite = new Sprite("LaserPointer.png");
	m_mouseSprite = new Sprite("MouseToy.png");
	m_brushSprite = new Sprite("Brush.png");
}

HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::Update(float deltaTime)
{
	m_timeSinceLastMouseClick += deltaTime;

	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame(g_window);
	ImGui::NewFrame();

	ImGui::Begin("Debug Console");
	ImGui::SetWindowPos(ImVec2(0, 384));
	ImGui::SetWindowSize(ImVec2(480, 256));

	if (ImGui::CollapsingHeader("Log"))
	{
		for (std::string entry : m_logEntries)
		{
			ImGui::Text(entry.c_str());
		}
	}
	
	for (Cat* cat : LevelHandler::GetActiveLevel()->GetCats())
	{
		bool showThisCat = ImGui::CollapsingHeader(cat->m_name.c_str());

		if (showThisCat)
		{
			ImGui::Text(cat->m_name.c_str());
			std::string currentEmotion = "Current Emotion: " + g_sMoodValues[cat->m_currentEmotion];
			ImGui::Text(currentEmotion.c_str());
			ImGui::InputFloat4("Emotional State", &cat->m_emotionalState.m_emotionalAxis[0]);

			ImGui::Text("Personality: ");
			std::string openness = "Openness: " + std::to_string(cat->m_personality.GetAxis(Openness));
			std::string conscientiousness = "Conscientiousness: " + std::to_string(cat->m_personality.GetAxis(Conscientiousness));
			std::string extroversion = "Extroversion: " + std::to_string(cat->m_personality.GetAxis(Extroversion));
			std::string agreeableness = "Agreeableness: " + std::to_string(cat->m_personality.GetAxis(Agreeableness));
			std::string neuroticism = "Neuroticism: " + std::to_string(cat->m_personality.GetAxis(Neuroticism));
			ImGui::Text(openness.c_str());
			ImGui::Text(conscientiousness.c_str());
			ImGui::Text(extroversion.c_str());
			ImGui::Text(agreeableness.c_str());
			ImGui::Text(neuroticism.c_str());
		}
	}

	ImGui::End();

	ImGui::Begin("Tools");
	ImGui::SetWindowPos(ImVec2(480, 384));
	ImGui::SetWindowSize(ImVec2(160, 256));

	for (int i = Tools::Tool_PetCat; i < NumTools; i++)
	{
		std::string title = (m_equippedTool == i ? "Unequip " : "Equip ") + s_Tools[i];

		if (ImGui::Button(title.c_str()))
		{
			if (m_equippedTool == i)
				m_equippedTool = Tools::Tool_None;
			else
				m_equippedTool = i;
		}
	}

	ImGui::End();
}

void HeadsUpDisplay::Render()
{
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

	Vector2 mousePos = InputHandler::GetMousePosition();

	switch (m_equippedTool)
	{
	case (Tool_PetCat):
	{
		Vector2 size = Vector2(32, 32);
		Vector2 pos = mousePos - Vector2(16, 16);
		if (m_timeSinceLastMouseClick < 0.25f)
		{
			pos += Vector2((rand() % 8) - 4, (rand() % 8) - 4);
		}
		Vector2 offset = Vector2(0, 0);

		m_petSprite->Render(pos, size, offset, size);
	}
	break;
	case (Tool_Vacuum):
	{
		Vector2 size = Vector2(32, 32);
		Vector2 pos = mousePos - Vector2(16, 16);
		if (m_timeSinceLastMouseClick < 0.25f)
		{
			m_vacuumFrame += 1;
			m_vacuumFrame %= 3;
		}
		else
		{
			m_vacuumFrame = 0;
		}
		Vector2 offset = Vector2(0 + ((float)m_vacuumFrame * 32.0f), 0);
		m_vacuumSprite->Render(pos, size, offset, size);
	}
	break;
	case (Tool_Toy):
	{
		Vector2 size = Vector2(32, 32);
		Vector2 pos = mousePos - Vector2(24, 24);
		if (m_timeSinceLastMouseClick < 0.25f)
		{
			m_toyFrame += 1;
			m_toyFrame %= 4;
		}
		else
		{
			m_toyFrame = 0;
		}

		Vector2 offset = Vector2(0 + ((float)m_toyFrame * 32.0f), 0);
		m_toySprite->Render(pos, size, offset, size);
	}
	break;
	case (Tool_SprayBottle):
	{
		Vector2 size = Vector2(32, 32);
		Vector2 pos = mousePos - Vector2(16, 8);
		Vector2 offset = Vector2(0, 0);
		if (m_timeSinceLastMouseClick < 0.25)
		{
			offset.x += 32.0f;
		}
		m_sprayBottleSprite->Render(pos, size, offset, size);
	}
	break;
	case (Tool_Mouse):
	{
		Vector2 size = Vector2(32, 32);
		Vector2 pos = mousePos - Vector2(16, 8);
		if (m_timeSinceLastMouseClick < 0.25f)
		{
			pos += Vector2((rand() % 8) - 4, (rand() % 8) - 4);
		}
		Vector2 offset = Vector2(0, 0);
		m_mouseSprite->Render(pos, size, offset, size);
	}
	break;
	case (Tool_Cucumber):
	{
		Vector2 size = Vector2(32, 32);
		Vector2 pos = mousePos - Vector2(16, 16);
		if (m_timeSinceLastMouseClick < 0.25f)
		{
			pos += Vector2((rand() % 8) - 4, (rand() % 8) - 4);
		}
		Vector2 offset = Vector2(0, 0);
		m_cucumberSprite->Render(pos, size, offset, size);
	}
	break;
	case (Tool_LaserPointer):
	{
		Vector2 size = Vector2(32, 32);
		Vector2 pos = mousePos - Vector2(16, 16);
		if (m_timeSinceLastMouseClick < 0.25f)
		{
			pos += Vector2((rand() % 8) - 4, (rand() % 8) - 4);
		}
		Vector2 offset = Vector2(0, 0);
		m_laserPointerSprite->Render(pos, size, offset, size);
	}
	break;
	case (Tool_Brush):
	{
		Vector2 size = Vector2(32, 32);
		Vector2 pos = mousePos - Vector2(16, 16);
		if (m_timeSinceLastMouseClick < 0.25f)
		{
			pos += Vector2((rand() % 8) - 4, (rand() % 8) - 4);
		}
		Vector2 offset = Vector2(0, 0);
		m_brushSprite->Render(pos, size, offset, size);
	}
	break;
	}

	m_mousePositionLastFrame = mousePos;
}

void HeadsUpDisplay::MouseButtonDown()
{
	m_timeSinceLastMouseClick = 0.0f;
	m_mouseButtonDown = true;
	Vector2 mousePos = InputHandler::GetMousePosition();

	switch (m_equippedTool)
	{
	case (Tool_Toy):
	{
		AudioHandler::PlaySoundEffect(SFX_Toy);
		std::vector<Cat*> nearbyCats = LevelHandler::GetActiveLevel()->GetAllCatsWithinRadius(mousePos, 64.0f);
		EmotionalEventHandler::SendEmotionalEvent(EmotionalState(1, 0, 0, 0), nearbyCats);
	}
	break;
	case(Tool_SprayBottle):
	{
		std::vector<Cat*> nearbyCats = LevelHandler::GetActiveLevel()->GetAllCatsWithinRadius(mousePos, 64.0f);
		AudioHandler::PlaySoundEffect(SFX_Spray);
		EmotionalEventHandler::SendEmotionalEvent(EmotionalState(-1, 0, 0, 0), nearbyCats);
	}
	break;
	case(Tool_PetCat):
	{
		std::vector<Cat*> nearbyCats = LevelHandler::GetActiveLevel()->GetAllCatsWithinRadius(mousePos, 32.0f);
		EmotionalEventHandler::SendEmotionalEvent(EmotionalState(0, 1, 0, 0), nearbyCats);
	}
	break;
	case (Tool_Cucumber):
	{
		std::vector<Cat*> nearbyCats = LevelHandler::GetActiveLevel()->GetAllCatsWithinRadius(mousePos, 64.0f);
		EmotionalEventHandler::SendEmotionalEvent(EmotionalState(0, -1, 0, 0), nearbyCats);
	}
	break;
	case (Tool_Vacuum):
	{
		AudioHandler::PlaySoundEffect(SFX_Vacuum);
		std::vector<Cat*> nearbyCats = LevelHandler::GetActiveLevel()->GetAllCatsWithinRadius(mousePos, 64.0f);
		EmotionalEventHandler::SendEmotionalEvent(EmotionalState(0, 0, 1, 0), nearbyCats);
	}
	break;
	case (Tool_Brush):
	{
		std::vector<Cat*> nearbyCats = LevelHandler::GetActiveLevel()->GetAllCatsWithinRadius(mousePos, 64.0f);
		EmotionalEventHandler::SendEmotionalEvent(EmotionalState(0, 0, -1, 0), nearbyCats);
	}
	break;
	case (Tool_LaserPointer):
	{
		std::vector<Cat*> nearbyCats = LevelHandler::GetActiveLevel()->GetAllCatsWithinRadius(mousePos, 64.0f);
		EmotionalEventHandler::SendEmotionalEvent(EmotionalState(0, 0, 0, 1), nearbyCats);
	}
	break;
	case (Tool_Mouse):
	{
		std::vector<Cat*> nearbyCats = LevelHandler::GetActiveLevel()->GetAllCatsWithinRadius(mousePos, 64.0f);
		EmotionalEventHandler::SendEmotionalEvent(EmotionalState(0, 0, 0, -1), nearbyCats);
	}
	break;
	}
}

void HeadsUpDisplay::MouseButtonUp()
{
	m_mouseButtonDown = false;
}

void HeadsUpDisplay::PushLogEntry(std::string entry)
{
	m_logEntries.push_back(entry);
}
