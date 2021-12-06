#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_sdlrenderer.h"
#include "InterfaceHandler.h"
#include <SDL.h>

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
	ImGui::Text("Test test test test test test test kjbjaskgjhsagha");
	ImGui::End();
}

void HeadsUpDisplay::Render()
{
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}
