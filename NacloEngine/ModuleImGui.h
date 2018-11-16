#ifndef __MODULEIMGUI_H__
#define __MODULEIMGUI_H__

#include "Module.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "ImGui/imgui_impl_sdl.h"
#include "PCG/pcg_variants.h"
#include "PCG/entropy.h"
#include "ImGuizmo/ImGuizmo.h"

#define MAX_SNAKE 2

class Panel;
class PanelConsole;
class PanelConfiguration;
class PanelAbout;
class PanelRandomNumberGenerator;
class PanelInspector;
class PanelHierarchy;
class PanelResources;

enum TypeSave;

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

	void ShowImGuiInfo();


public:

	PanelConsole* console = nullptr;
	PanelConfiguration* configuration = nullptr;
	PanelAbout* about = nullptr;
	PanelRandomNumberGenerator* randomNumberGenerator = nullptr;
	PanelInspector* inspector = nullptr;
	PanelHierarchy* hierarchy = nullptr;
	PanelResources* resources = nullptr;


	bool wire_mode = false;

private:

	bool Show_Main_Menu_Bar();
	void const ImGuiShortCuts();
	ImGuizmo::OPERATION guizmoOperation = ImGuizmo::TRANSLATE;

private:
	std::vector<Panel*> panels;

	float clear_color = 0.5f;
	std::string explorer_path;
	bool close_engine = false; 
};

#endif