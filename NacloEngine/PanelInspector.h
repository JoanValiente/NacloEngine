#ifndef __PANELINSPECTOR_H__
#define __PANELINSPECTOR_H__

#include "Panel.h"

struct GameObject;

class PanelInspector : public Panel
{
public:
	PanelInspector();
	virtual ~PanelInspector();

	void DrawInspector();

private:

};

#endif// __PANELINSPECTOR_H__
