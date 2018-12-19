#include "OpenGL.h"
#include "ComponentLabel.h"
#include "Component.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "ComponentRectTransform.h"
#include "SDL2_ttf/include/SDL_ttf.h"


ComponentLabel::ComponentLabel(GameObject * container) : Component (container)
{
	glGenTextures(1, &id_font);
	RELEASE_ARRAY(input_text);
	ReSizeInput();
}

ComponentLabel::~ComponentLabel()
{
}

void ComponentLabel::Update(float dt)
{
}

void ComponentLabel::ShowInspector()
{
	if (ImGui::CollapsingHeader("Label"))
	{
		if (ImGui::InputTextMultiline("Text", input_text, max_input, ImVec2(ImGui::GetWindowWidth(), 100), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine))
		{
			SetString(input_text);
		}
	}
}

void ComponentLabel::FreeFont()
{
}

void ComponentLabel::Clear()
{
}

void ComponentLabel::SetString(std::string input)
{
	if (input.size() > max_input) {
		return;
	}
	text_str = input;
	//GenerateMesh();
}

int ComponentLabel::GetWidth() const
{
	return text_width;
}

int ComponentLabel::GetHeight() const
{
	return text_height;
}

void ComponentLabel::UpdateText()
{
	if (text == nullptr)
		return;
	if (!text->font || text_str.empty())
		return;
	else if (s_font != NULL && text_str.empty())
	{
		FreeFont();
		return;
	}
	if (s_font != NULL)
	{
		FreeFont();
	}
	update_text = true;
	text->size = text_size;
	//text->ReLoadToMemory();
	TTF_SizeText(text->font, text_str.c_str(), &text_width, &text_height);

	s_font = TTF_RenderText_Blended_Wrapped(text->font, text_str.c_str(), SDL_Color{ (Uint8)(255), (Uint8)(255),(Uint8)(255), (Uint8)(255) }, text_width);

	glBindTexture(GL_TEXTURE_2D, id_font);
	SetTextureID(id_font);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_width, text_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, s_font->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(s_font);
}

bool ComponentLabel::GenerateText()
{
	if (text_str.empty())
		return false;

	float width = container->rectTransform->width;
	float height = container->rectTransform->height;
	float4 rect_transform = float4(-width, -height, width, height);

	if (TextCanFit(rect_transform))
	{
		//Generate quad pos with horizontal and vertical pos
		//Just put it in the middle for now
		//Need to generate quad vertexs and indexs
	}
}

bool ComponentLabel::TextCanFit(float4 rect_transform)
{
	if (abs(rect_transform.x) + abs(rect_transform.z) > text_width&&abs(rect_transform.y) + abs(rect_transform.w) > text_height)
	{
		return true;
	}
	return false;
}

void ComponentLabel::ReSizeInput()
{
	std::string tmp;
	tmp.clear();
	if (input_text != nullptr)
		tmp = input_text;
	RELEASE_ARRAY(input_text);
	input_text = new char[max_input];
	strcpy_s(input_text, max_input, tmp.c_str());
}

void ComponentLabel::ExpandMesh()
{
	UpdateText();
	GenerateText();
}
