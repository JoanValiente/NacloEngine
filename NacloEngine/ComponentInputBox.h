#ifndef __ComponentInputBox_H__
#define __ComponentInputBox_H__

#include "Component.h"
#include "ComponentInteractive.h"

struct ComponentLabel;
struct ComponentImage;

class ComponentInputBox : public ComponentInteractive
{
public:
	ComponentInputBox(GameObject* container);
	~ComponentInputBox();
	void Update(float dt);
	void ShowInspector();
	//int GetWidth()const;
	//int GetHeight()const;
	void Hover();
	void Enter();
	void Exit();
	void OnClick();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

public:
	//ComponentImage* image = nullptr;
	float2 size = { 50, 10 };
	char* newText = nullptr;
	std::string composition;
	ComponentLabel* text = nullptr;
	ComponentImage* image = nullptr;
	int horizontalMargin = 0;
	int verticalMargin = 0;
};
#endif //__ComponentInputBox_H__
