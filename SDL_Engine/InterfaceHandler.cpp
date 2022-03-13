#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_sdlrenderer.h"
#include "InterfaceHandler.h"
#include <SDL.h>
#include "Cat.h"
#include "LevelHandler.h"
#include "Level.h"
#include "EmotionUtilities.h"

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
{
}

HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::Update(float deltaTime)
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame(g_window);
	ImGui::NewFrame();

	ImGui::Begin("Test");
	ImGui::SetWindowPos(ImVec2(0, 384));
	ImGui::SetWindowSize(ImVec2(640, 256));

	if (ImGui::Button("Stimulate Joy"))
	{
		EmotionalState impulse;
		impulse[JoySadness] = 1.0f;
		EmotionalEvent* e = new EmotionalEvent(impulse);
		EmotionalEventHandler::m_bus->publish(e);
		delete e;
	}
	
	for (Cat* cat : LevelHandler::GetActiveLevel()->GetCats())
	{
		bool showThisCat = ImGui::CollapsingHeader(cat->m_name.c_str());

		if (showThisCat)
		{
			ImGui::Text(cat->m_name.c_str());
			std::string currentEmotion = "Current Emotion: " + IdentifyEmotionalState(cat->m_emotionalState).second;
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
}

void HeadsUpDisplay::Render()
{
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}
