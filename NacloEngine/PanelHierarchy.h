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
	bool ShowGameObjectHierarchy(GameObject* go);

private:
	bool move = false;
	GameObject* toMove = nullptr;
	GameObject* target = nullptr;
};

#endif// __PANELHIERARCHY_H__
