#ifndef _ComponentImage_H_
#define _ComponentImage_H_

struct Texture;
class plane;

#include "MathGeoLib/MathGeoLib.h"

struct ImagePlane
{
	float3 vertex[4];
	float2 uv[4];
	uint index[6]{
		2,1,0,
		3,1,2 
	};

	uint indexId = 0;
	uint vertexId = 0;
	uint textureId = 0;
};

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* container);
	~ComponentImage();

	void ShowInspector();
	void AssignTexture(Texture* texture);

	void Render(uint texture_id = 0);

	void UpdateImagePlane();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

private:
	void CreateImagePlane();
	Texture* CreateEmptyTexture();
public:
	Texture * tex = nullptr;
	ImagePlane plane;

private:
	float alpha = 1.0f;
	bool preserveAspect = false;
};

#endif

