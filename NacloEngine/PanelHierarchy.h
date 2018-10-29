#ifndef __PANELHIERARCHY_H__
#define __PANELHIERARCHY_H__

#include "Panel.h"

struct GameObject;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy();
	virtual ~PanelHierarchy();

	void Draw();
	void ShowGameObjectHierarchy(GameObject* go);
};

#endif// __PANELHIERARCHY_H__
