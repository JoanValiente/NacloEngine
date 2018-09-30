#ifndef __PANELOPENGL_H__
#define __PANELOPENGL_H__

#include "Panel.h"

class PanelOpenGl : public Panel
{
public:
	PanelOpenGl();
	virtual ~PanelOpenGl();

	void Draw();

private:
	bool test = true;
};

#endif// __PANELOPENGL_H__
