#ifndef __PANELCONSOLE_H__
#define __PANELCONSOLE_H__

#include "Panel.h"
#include "Imgui/imgui.h"

class PanelConsole : public Panel
{
public:
	PanelConsole();
	virtual ~PanelConsole();

	void Draw();

	void Clear();
	void ConsoleLog(const char* text);

private:
	ImGuiTextBuffer console_logs;
	bool ConsoleScroll;
};

#endif  // __PANELCONSOLE_H__
