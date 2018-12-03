#ifndef _ComponentImage_H_
#define _ComponentImage_H_

#include "ComponentUI.h"
#include "SDL\include\SDL.h"

class ComponentImage : public ComponentUI
{
public:
	ComponentImage(GameObject* container, float x, float y, UI_TYPE type, SDL_Texture* texture, ComponentUI* parent = nullptr);

	void ShowInspector();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);
};

#endif

