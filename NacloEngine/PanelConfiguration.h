#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

#include "Panel.h"
#include "Imgui/imgui.h"
#include <vector>
#include "OpenGL.h"

#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX			0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX		0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX    0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX				0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX				0x904B

#define FPS_LOG_SIZE 100
#define PCG_VERSION "0.98"
#define MATHGEOLIB_VERSION "1.5"

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration();
	virtual ~PanelConfiguration();

	void Draw();

	void LoadHardwareInfo();

private:
	void const ShowHardwareInfo();

private:
	char sdl_version[10];
	const char* imgui_version = nullptr;
	char glew_version[10];
	char assimp_version[10];
	char devil_version[10];
	const char* mathgeolib_version = nullptr;
	const char* pcg_version = nullptr;

	int devil_version_num;

	const GLubyte *vendor = nullptr;
	const GLubyte *gpu_renderer = nullptr;

	ImVec4 color; 

};
#endif