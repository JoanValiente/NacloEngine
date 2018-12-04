#ifndef _ComponentImage_H_
#define _ComponentImage_H_

#include "SDL\include\SDL.h"


class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* container);

	void ShowInspector();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

private:
	SDL_Texture * image;
};

#endif

