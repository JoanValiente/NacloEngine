#ifndef __ComponentUI_H__
#define __ComponentUI_H__

#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Component.h"
#include "SDL/include/SDL.h"

enum UI_TYPE
{
	UNKNOWN,
	CANVAS,
	IMAGE,
	BUTTON,
	LABEL
};

enum UI_STATE;
class Module;

class ComponentUI :  public Component 
{
public:
	ComponentUI(GameObject * container, UI_TYPE type, ComponentUI* parent = nullptr);
	~ComponentUI();

	virtual void Draw(float dt);
	virtual void Update(float dt);

	UI_TYPE GetType() const;

	void AddChildren(ComponentUI* children);

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

public:
	bool active = true;
	ComponentUI* parent = nullptr;
	Module* callback = nullptr;

	std::vector<ComponentUI*> children;
	SDL_Rect* rect;
	float3 pos;
	UI_STATE state;

	const SDL_Texture *texture = nullptr;

private:
	UI_TYPE type;
	bool mouseHover;
};

#endif // !__UIElement_H__
