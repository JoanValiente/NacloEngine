#include "OpenGL.h"
#include "ComponentLabel.h"
#include "Component.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "ComponentRectTransform.h"
#include "ModuleRenderer3D.h"
#include "TextureImporter.h"
#include "ModuleFonts.h"
#include "SDL2_ttf/include/SDL_ttf.h"
#include "Config.h"


ComponentLabel::ComponentLabel(GameObject * container) : Component (container)
{
	glGenTextures(1, &id_font);
	RELEASE_ARRAY(input_text);
	ReSizeInput();
	tex = new Texture;
	this->type = COMPONENT_LABEL;

	if (container->rectTransform != nullptr)
	{
		if (!container->rectTransform->loading)
		{
			container->rectTransform->SetHeight(size.y);
			container->rectTransform->SetWidth(size.x);
		}
		text_width = container->rectTransform->width;
		text_height = container->rectTransform->height;


		CreateLabelPlane();
		container->rectTransform->UpdateMatrix();
	}
	text = App->fonts->Load(DEFAULT_FONT, 48);
	text_str = "Label";
}

ComponentLabel::~ComponentLabel()
{
	TTF_CloseFont(text->font);
	delete text;
}

void ComponentLabel::Update(float dt)
{
	if (container->rectTransform != nullptr) {
		GenerateText();
		UpdateText();
		PreserveAspect();
	}
}

void ComponentLabel::ShowInspector()
{
	int newSize = text->size;
	int newMaxInput = max_input;

	if (ImGui::CollapsingHeader("Label"))
	{
		ImGui::Text("Max Input");
		ImGui::SameLine();
		if (ImGui::DragInt("##maxInput", &newMaxInput, 1)) {
			max_input = newMaxInput;
		}

		if (ImGui::InputTextMultiline("Text", input_text, max_input, ImVec2(ImGui::GetWindowWidth(), 100), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine))
		{
			SetString(input_text);
		}

		ImGui::Text("Size");
		ImGui::SameLine();
		if (ImGui::DragInt("##size", &newSize, 1)) {
			TTF_CloseFont(text->font);
			text = nullptr;
			text = App->fonts->Load(DEFAULT_FONT, newSize);
		}

		ImGui::ColorEdit4("Color##image_rgba", color.ptr());
	}
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

	update_text = true;
	//text->size = text_size;
	TTF_SizeText(text->font, text_str.c_str(), &text_width, &text_height);
	if (!first_time)
	{
		aux_text_width = text_width;
		aux_text_height = text_height;
		ratio = container->rectTransform->width / aux_text_width;
		ratio_2 = container->rectTransform->height / aux_text_height;

		first_time = true;
	}

	s_font = TTF_RenderText_Blended_Wrapped(text->font, text_str.c_str(), SDL_Color{ (Uint8)(color.x * 255), (Uint8)(color.y * 255),(Uint8)(color.z * 255), (Uint8)(color.w * 255) }, text_width);

	glBindTexture(GL_TEXTURE_2D, id_font);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_width, text_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, s_font->pixels);
	tex->texture_id = id_font;
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(s_font);
}

void ComponentLabel::Render(uint texture_id)
{
	glPushMatrix();
	UpdateLabelPlane();
	float4x4 matrix = container->rectTransform->globalMatrix;
	glMultMatrixf((GLfloat*)matrix.Transposed().ptr());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(255, 255, 255, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glBindBuffer(GL_ARRAY_BUFFER, plane.vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, plane.textureId);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane.indexId);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

bool ComponentLabel::GenerateText()
{
	if (text_str.empty()) {
		return false;
	}

	float width = container->rectTransform->width;
	float height = container->rectTransform->height;
	float4 rect_transform = float4(-width, -height, width, height);

	if (TextCanFit(rect_transform))
	{
		//Generate quad pos with horizontal and vertical pos
		//Just put it in the middle for now
		//Need to generate quad vertexs and indexs
		UpdateLabelPlane();
	}
}

bool ComponentLabel::TextCanFit(float4 rect_transform)
{
	if (abs(rect_transform.x) + abs(rect_transform.z) > text_width && abs(rect_transform.y) + abs(rect_transform.w) > text_height)
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

void ComponentLabel::PreserveAspect()
{
	if (text_width != aux_text_width)
	{
		container->rectTransform->width = text_width * ratio;
		aux_text_width = text_width;
	}
	if (text_height != aux_text_height)
	{
		container->rectTransform->height = text_height * ratio_2;
		aux_text_height = text_height;
	}
}

void ComponentLabel::CreateLabelPlane()
{
	float width = container->rectTransform->GetWidth();
	float height = container->rectTransform->GetHeight();
	plane.vertex[0] = float3(width / 2, -height / 2, 0);
	plane.uv[0] = float2(1, 0);

	plane.vertex[1] = float3(width / 2, height / 2, 0);
	plane.uv[1] = float2(1, 1);

	plane.vertex[2] = float3(-width / 2, -height / 2, 0);
	plane.uv[2] = float2(0, 0);

	plane.vertex[3] = float3(-width / 2, height / 2, 0);
	plane.uv[3] = float2(0, 1);

	glGenBuffers(1, (GLuint*)&plane.vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, plane.vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, plane.vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, (GLuint*)&plane.textureId);
	glBindBuffer(GL_ARRAY_BUFFER, plane.textureId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, plane.uv, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&plane.indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane.indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 6, plane.index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void ComponentLabel::UpdateLabelPlane()
{
	float width = container->rectTransform->GetWidth();
	float height = container->rectTransform->GetHeight();

	plane.vertex[0] = float3(width / 2, -height / 2, 0);
	plane.vertex[1] = float3(width / 2, height / 2, 0);
	plane.vertex[2] = float3(-width / 2, -height / 2, 0);
	plane.vertex[3] = float3(-width / 2, height / 2, 0);

	glBindBuffer(GL_ARRAY_BUFFER, plane.vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, plane.vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentLabel::SaveComponent(Config & conf)
{
	conf.SetFloat4("Color", color);
	conf.SetBool("Update Text", update_text);
	conf.SetInt("Width", text_width);
	conf.SetInt("Height", text_height);
	conf.SetInt("Max Input", max_input);
	conf.SetInt("Text Size", text->size);
	conf.SetString("Text", text_str.c_str());
}

void ComponentLabel::LoadComponent(Config & conf)
{
	color		= conf.GetFloat4("Color");
	update_text	= conf.GetBool("Update Text");
	text_width	= conf.GetInt("Width");
	text_height	= conf.GetInt("Height");
	max_input	= conf.GetInt("Max Input");
	text->size  = conf.GetInt("Text Size");
	text_str	= (char*)conf.GetString("Text");
}
