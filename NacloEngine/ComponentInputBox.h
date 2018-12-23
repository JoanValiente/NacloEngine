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
	char* newText = nullptr;
	std::string composition;
	ComponentLabel* text = nullptr;
	float horizontalMargin = 0.0f;
	float verticalMargin = 0.0f;
};
#endif //__ComponentInputBox_H__
