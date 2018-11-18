#ifndef __PANELTIME_H__
#define __PANELTIME_H__
			   
#include "Panel.h"

class PanelTime : public Panel
{
public:
	PanelTime();
	virtual ~PanelTime();

	void Draw();

private:
	float timeScale = 1.0f;
};

#endif
