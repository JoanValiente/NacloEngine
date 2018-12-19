#ifndef __ComponentLabel_H__
#define __ComponentLabel_H__

struct SDL_Surface;
typedef struct _TTF_Font TTF_Font;

#include "MathGeoLib/Math/float4.h"
#include <string>
#include "Component.h"

struct Font
{
	TTF_Font* font = NULL;
	int size = 12;
};

class ComponentLabel : public Component
{
public:
	ComponentLabel(GameObject* container);
	~ComponentLabel();
	void Update(float dt);
	void ShowInspector();
	void FreeFont();
	void Clear();
	void SetString(std::string input);
	int GetWidth()const;
	int GetHeight()const;

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

private:

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
