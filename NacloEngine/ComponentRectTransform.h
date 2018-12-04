#ifndef _ComponentRectTransform_H_
#define _ComponentRectTransform_H_

class ComponentRectTransform : public Component
{
public:
	ComponentRectTransform(GameObject* container);

	void ShowInspector();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

};

#endif
