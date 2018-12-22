#ifndef __ComponentLabel_H__
#define __ComponentLabel_H__

struct SDL_Surface;
struct Texture;
struct Font;
typedef struct _TTF_Font TTF_Font;

#include "MathGeoLib/Math/float4.h"
#include "MathGeoLib/Math/float2.h"
#include <string>
#include "Component.h"

struct LabelPlane
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

class ComponentLabel : public Component
{
public:
	ComponentLabel(GameObject* container);
	~ComponentLabel();
	void Update(float dt);
	void ShowInspector();
	void Clear();
	void SetString(std::string input);
	int GetWidth()const;
	int GetHeight()const;
	void CreateLabelPlane();
	void UpdateLabelPlane();
	void Render(uint texture_id);

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

private:
	void UpdateText();
	bool GenerateText();
	bool TextCanFit(float4 rect_transform);
	void ReSizeInput();
	void ExpandMesh();

public:
	SDL_Surface * s_font = nullptr;
	Font* text = nullptr;
	std::string text_str;
	LabelPlane plane;
	Texture* tex;


private:

	float4 color = float4::one;
	bool update_text = false;
	int text_width = 0;
	int text_height = 0;

	//std::string input_text;
	char* input_text = nullptr;
	int max_input = 20;
	int text_size = 12;
	uint id_font = 0;
};
#endif //__ComponentLabel_H__
