#include "PanelConsole.h"
#include "ModuleInput.h"

// ---------------------------------------------------------
PanelConsole::PanelConsole() : Panel("Console")
{
	active = false;
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
	ImGui::Begin("Console", &active);
	ImGui::TextUnformatted(console_logs.begin());
	if (ConsoleScroll)
		ImGui::SetScrollHere(1.0f);
	ConsoleScroll = false;
	ImGui::End();
}