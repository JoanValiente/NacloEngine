#ifndef _ComponentImage_H_
#define _ComponentImage_H_

#include "Component.h"
#include "SDL\include\SDL.h"

class ComponentImage : public Component
{
public:
	ComponentImage(int x, int y, uint type, SDL_Texture* texture, Component* parent = nullptr);

	void ShowInspector();
};

#endif


