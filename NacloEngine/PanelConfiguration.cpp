#include "PanelConfiguration.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "mmgr\mmgr.h"
#include "Assimp/include/version.h"

#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX			0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX		0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX    0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX				0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX				0x904B

PanelConfiguration::PanelConfiguration() : Panel("Configuration")
{
	active = true;

}

// ---------------------------------------------------------
PanelConfiguration::~PanelConfiguration()
{}

void PanelConfiguration::Draw()
{
	if (!ImGui::Begin("Configuration", &active))
	{
		ImGui::End();
	}
	else
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			App->ShowApplicationInfo();
		}


		if (ImGui::CollapsingHeader("Window"))
		{
			App->window->ShowWindowInfo();
		}

		if (ImGui::CollapsingHeader("Camera"))
		{
			App->camera->ShowCameraInfo();
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			App->input->ShowInputInfo();
		}

		if (ImGui::CollapsingHeader("Render"))
		{
			App->renderer3D->ShowRenderInfo();
		}

		if (ImGui::CollapsingHeader("ImGui"))
		{
			App->imgui->ShowImGuiInfo();
		}

		if (ImGui::CollapsingHeader("Textures"))
		{
			App->texture->ShowTextureInfo();
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ShowHardwareInfo();
		}
		ImGui::End();
	}
}

void PanelConfiguration::LoadHardwareInfo()
{
	SDL_version curren_version;
	SDL_VERSION(&curren_version);
	sprintf_s(sdl_version, sizeof(sdl_version), "%d.%d.%d", curren_version.major, curren_version.minor, curren_version.patch);

	imgui_version = ImGui::GetVersion();

	sprintf_s(glew_version, "%i.%i.%i", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

	sprintf_s(assimp_version, "%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());

	mathgeolib_version = MATHGEOLIB_VERSION;

	pcg_version = PCG_VERSION;

	devil_version_num = ilGetInteger(IL_VERSION_NUM);
	sprintf_s(devil_version, sizeof(devil_version), "%i.%i.%i", devil_version_num / 100, (devil_version_num / 10) - (devil_version_num * 10 / 100), devil_version_num - (devil_version_num / 10) * 10);

	color = { 1, 0.7f, 0, 100 };

	vendor = glGetString(GL_VENDOR);
	gpu_renderer = glGetString(GL_RENDERER);
	
	glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_memory);
	glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available_memory);
	glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicated_memory);

	memory_usage = total_memory - available_memory;
}

void const PanelConfiguration::ShowHardwareInfo()
{
	ImGui::Text("SDL Version: ");
	ImGui::SameLine();

	ImGui::TextColored(color, sdl_version);

	ImGui::Text("ImGui Version: ");
	ImGui::SameLine();

	ImGui::TextColored(color, imgui_version);

	ImGui::Text("Glew Version: ");
	ImGui::SameLine();

	ImGui::TextColored(color, glew_version);

	ImGui::Text("Assimp Version: ");
	ImGui::SameLine();

	ImGui::TextColored(color, assimp_version);

	ImGui::Text("MathGeolib Version: ");
	ImGui::SameLine();

	ImGui::TextColored(color, mathgeolib_version);

	ImGui::Text("PCG Version: ");
	ImGui::SameLine();

	ImGui::TextColored(color, pcg_version);

	ImGui::Text("Devil Version: ");
	ImGui::SameLine();
	ImGui::TextColored(color, devil_version);

	ImGui::Separator();

	ImGui::Text("CPUs: ");
	ImGui::SameLine();

	int cpu = SDL_GetCPUCount();
	char cpu_version[10];
	sprintf_s(cpu_version, sizeof(sdl_version), "%i", cpu);
	ImGui::TextColored(color, cpu_version);


	ImGui::Text("System RAM: ");
	ImGui::SameLine();

	cpu = SDL_GetSystemRAM() / 1000;
	sprintf_s(cpu_version, sizeof(sdl_version), "%i Gb", cpu);
	ImGui::TextColored(color, cpu_version);

	ImGui::Text("Caps: ");
	ImGui::SameLine();

	if (SDL_HasRDTSC())
	{
		ImGui::TextColored(color, "RDTSC");
		ImGui::SameLine();
	}
	if (SDL_HasMMX())
	{
		ImGui::TextColored(color, "MMX");
		ImGui::SameLine();
	}
	if (SDL_HasAVX())
	{
		ImGui::TextColored(color, "AVX");
		ImGui::SameLine();
	}
	if (SDL_HasAVX2())
	{
		ImGui::TextColored(color, "AVX2");
		ImGui::SameLine();
	}
	if (SDL_HasSSE())
	{
		ImGui::TextColored(color, "SSE");
		ImGui::SameLine();
	}
	if (SDL_HasSSE2())
	{
		ImGui::TextColored(color, "SSE2");
		ImGui::SameLine();
	}
	if (SDL_HasSSE3())
	{
		ImGui::TextColored(color, "SSE3");
		ImGui::SameLine();
	}
	if (SDL_HasSSE41())
	{
		ImGui::TextColored(color, "SSE41");
		ImGui::SameLine();
	}
	if (SDL_HasSSE42())
	{
		ImGui::TextColored(color, "SSE42");
	}

	ImGui::Separator();

	//-------------GPU-------------

	ImGui::Text("GPU: ");
	ImGui::SameLine();
	ImGui::TextColored(color, "%s", vendor);

	ImGui::Text("Brand: ");
	ImGui::SameLine();
	ImGui::TextColored(color, "%s", gpu_renderer);

	//-------------VRAM-------------

	ImGui::Separator();

	ImGui::Text("VRAM Budget: "); ImGui::SameLine();
	ImGui::TextColored(color, "%.4f MB", (total_memory * 0.001));

	ImGui::Text("VRAM Usage: "); ImGui::SameLine();
	ImGui::TextColored(color, "%.4f MB", (memory_usage * 0.001));

	ImGui::Text("VRAM Available: "); ImGui::SameLine();
	ImGui::TextColored(color, "%.4f MB", (available_memory * 0.001));

	ImGui::Text("VRAM Reserved: "); ImGui::SameLine();
	ImGui::TextColored(color, "%.4f MB", (dedicated_memory * 0.001));
}

