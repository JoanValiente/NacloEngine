#ifndef __MODULEIMGUI_H__
#define __MODULEIMGUI_H__

#include "Module.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "ImGui/imgui_impl_sdl.h"
#include "PCG/pcg_variants.h"
#include "PCG/entropy.h"

#define MAX_SNAKE 2

class Panel;
class PanelConsole;
class PanelConfiguration;
class PanelAbout;
class PanelIntersections;
class PanelRandomNumberGenerator;
class PanelOpenGl;

class ModuleImgui : public Module
{
public:
	ModuleImgui(Application* app, bool start_enabled = true);
	~ModuleImgui();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();

	void Log(const char* text);

public:

	PanelConsole* console = nullptr;
	PanelConfiguration* configuration = nullptr;
	PanelAbout* about = nullptr;
	PanelIntersections* intersections = nullptr;
	PanelRandomNumberGenerator* randomNumberGenerator = nullptr;
	PanelOpenGl* opengloptions = nullptr;

private:

	bool Show_Main_Menu_Bar();

private:
	std::vector<Panel*> panels;

	float clear_color = 0.5f;

	bool show_demo_window = true;

	bool close_engine = false; 
};

#endif