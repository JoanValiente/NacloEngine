#ifndef _ComponentImage_H_
#define _ComponentImage_H_

struct Texture;
class plane;

#include "ComponentInteractive.h"
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

class ComponentImage : public ComponentInteractive
{
public:
	ComponentImage(GameObject* container);
	~ComponentImage();

	void Update(float dt);

	void ShowInspector();
	void AssignTexture(Texture* texture);

	void Render(uint texture_id = 0);

	void UpdateImagePlane();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

private:
	void PreserveAspect();
	void CreateImagePlane();
	Texture* CreateEmptyTexture();
public:
	Texture * tex = nullptr;
	ImagePlane plane;
	float4 color = float4::one;

private:
	float2 size = { 40, 40 };

	bool preserveAspect = false;
	float aux_width = 0.0f;
	float aux_height = 0.0f;
};

#endif

