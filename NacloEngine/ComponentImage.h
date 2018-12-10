#ifndef _ComponentImage_H_
#define _ComponentImage_H_

#include "SDL\include\SDL.h"

struct Texture;
class plane;

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* container);

	void ShowInspector();
	void AssignTexture(Texture* texture);

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

public:
	plane* image_rect;
	Texture * tex = nullptr;

private:
	//aiColor4D color;
	bool preserveAspect = false;
};

#endif

