#ifndef __UIElement_H__
#define __UIElement_H__

#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "SDL/include/SDL.h"

enum UI_TYPE
{
	NONE,
	CANVAS,
	IMAGE,
	BUTTON,
	LABEL
};

enum UI_STATE;

class UIElement
{
public:
	UIElement(UI_TYPE type, UIElement* parent = nullptr);
	~UIElement();

	virtual void Draw(float dt);
	virtual void Update(float dt);

	UI_TYPE GetType() const;

	void AddChildren(UIElement* children);

public:
	bool active = true;
	UIElement* parent = nullptr;
	Module* callback = nullptr;

	std::vector<UIElement*> children;
	SDL_Rect* rect;
	float3 pos;
	UI_STATE state;

	const SDL_Texture *texture = nullptr;

private:
	UI_TYPE type;
	bool mouseHover;
};

#endif // !__UIElement_H__
