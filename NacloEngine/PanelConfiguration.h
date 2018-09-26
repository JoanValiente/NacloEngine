#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

#include "Panel.h"
#include "Imgui/imgui.h"
#include <vector>

#define FPS_LOG_SIZE 100


class PanelConfiguration : public Panel
{
public:
	PanelConfiguration();
	virtual ~PanelConfiguration();

	void Draw();

private:
	std::vector<float> vector_ms;
	std::vector<float> vector_fps;
	std::vector<float> vector_memory_usage;
};
#endif