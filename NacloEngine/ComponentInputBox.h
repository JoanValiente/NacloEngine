#ifndef __ComponentInputBox_H__
#define __ComponentInputBox_H__

#include "Component.h"

struct ComponentLabel;
struct ComponentImage;

class ComponentInputBox : public Component
{
public:
	ComponentInputBox(GameObject* container);
	~ComponentInputBox();
	void Update(float dt);
	void ShowInspector();
	//int GetWidth()const;
	//int GetHeight()const;

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

public:
	//ComponentImage* image = nullptr;
	char* newText = nullptr;
	std::string composition;
	ComponentLabel* text = nullptr;
};
#endif //__ComponentInputBox_H__
