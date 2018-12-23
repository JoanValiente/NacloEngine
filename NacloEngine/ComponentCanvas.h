#ifndef _ComponentCanvas_H_
#define _ComponentCanvas_H_

#include <vector>

struct ComponentInteractive;

class ComponentCanvas : public Component
{
public:
	ComponentCanvas(GameObject* container);
	~ComponentCanvas();

	void Update(float dt);

	void ShowInspector();

	void DebugDraw();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

public:
	std::vector<ComponentInteractive*> interactive_components;

private:
	void UpdateInteractive();
	void ChangeCambasState();
	void SetCanvasGameMode();
	void SetCanvasEditorMode();
	void SetCanvasPosition();
};

#endif
