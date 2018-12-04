#ifndef _ComponentCanvas_H_
#define _ComponentCanvas_H_

class ComponentCanvas : public Component
{
public:
	ComponentCanvas(GameObject* container);

	void ShowInspector();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

};

#endif
