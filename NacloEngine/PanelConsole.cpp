#include "PanelConsole.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

// ---------------------------------------------------------
PanelConsole::PanelConsole() : Panel("Console")
{
	active = true;
}

// ---------------------------------------------------------
PanelConsole::~PanelConsole()
{
}

// ---------------------------------------------------------
void PanelConsole::Clear()
{
	console_logs.clear();
}

// ---------------------------------------------------------
void PanelConsole::ConsoleLog(const char* text)
{
	console_logs.appendf(text);
	ConsoleScroll = true;
}

// ---------------------------------------------------------
void PanelConsole::Draw()
{
	int width;
	int height;
	SDL_GetWindowSize(App->window->window, &width, &height);

	ImGui::SetNextWindowPos(ImVec2(0, height/2 + height/4 - 35));
	ImGui::SetNextWindowSize(ImVec2(width, height / 4 + 10));

	ImGui::Begin("Console", &active);
	ImGui::TextUnformatted(console_logs.begin());
	if (ConsoleScroll)
		ImGui::SetScrollHere(1.0f);
	ConsoleScroll = false;
	ImGui::End();
}