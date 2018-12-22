#include "OpenGL.h"
#include "Component.h"
#include "ComponentImage.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"
#include "ComponentRectTransform.h"
#include "TextureImporter.h"
#include "ModuleRenderer3D.h"


ComponentImage::ComponentImage(GameObject* container) : Component(container)
{
	this->type = COMPONENT_IMAGE;

	if (container->rectTransform != nullptr)
	{
		container->rectTransform->SetHeight(10.0f);
		container->rectTransform->SetWidth(20.0f);

		CreateImagePlane();
		container->rectTransform->UpdateMatrix();

		tex = CreateEmptyTexture();
	}

	else
	{
		LOG("Error creating Image Rect, no rect transform component created");
	}
}

ComponentImage::~ComponentImage()
{
	delete tex;
}

void ComponentImage::ShowInspector()
{	
	if (ImGui::CollapsingHeader("Image"))
	{
		if (tex != nullptr)
		{
			ImGui::InputText("##Name", (char*)tex->texture_name.c_str(), 64);
			ImGui::Text("SIZE");
			ImGui::Text("Width: %i", tex->width); ImGui::SameLine();
			ImGui::Text("Height: %i", tex->height);

			ImGui::SliderFloat("Alpha", &alpha, 0.0f, 1.0f);

			if (ImGui::Button("Change Texture"))
			{
				ImGui::OpenPopup("Change_Texture");
			}
			ImGui::Image((ImTextureID)tex->texture_id, { 256,256 });

			if (ImGui::BeginPopup("Change_Texture"))
			{
				list<Texture*>::const_iterator item = App->texture->textures_loaded.begin();

				while (item != App->texture->textures_loaded.end())
				{
					if ((*item) != nullptr)
					{
						ImGui::Image((ImTextureID)(*item)->texture_id, { 25,25 });
						if (ImGui::IsItemClicked())
						{
							tex = (*item);
						}
					}
					item++;
				}
				ImGui::EndPopup();
			}
		}
		else
		{
			ImGui::Text("No texture assigned");

			if (ImGui::Button("Add Texture"))
			{
				ImGui::OpenPopup("Change_Texture");
			}
			ImGui::Image(0, { 256,256 });

			if (ImGui::BeginPopup("Change_Texture"))
			{
				list<Texture*>::const_iterator item = App->texture->textures_loaded.begin();

				while (item != App->texture->textures_loaded.end())
				{
					if ((*item) != nullptr)
					{
						ImGui::Image((ImTextureID)(*item)->texture_id, { 25,25 });
						if (ImGui::IsItemClicked())
						{
							tex->texture_name = (*item)->texture_name;
							tex->width = (*item)->width;
							tex->height = (*item)->height;
							tex->texture_id = (*item)->texture_id;
						}
					}
					item++;
				}
				ImGui::EndPopup();
			}
		}
	}
}

void ComponentImage::AssignTexture(Texture * texture)
{
	this->tex = texture;
}

void ComponentImage::Render(uint texture_id)
{
	glPushMatrix();
	UpdateImagePlane();
	float4x4 matrix = container->rectTransform->globalMatrix;
	glMultMatrixf((GLfloat*)matrix.Transposed().ptr());

	//TRANSPARENCI
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(255, 255, 255, alpha);

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
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}

void ComponentImage::UpdateImagePlane()
{
	float width = container->rectTransform->GetWidth();
	float height = container->rectTransform->GetHeight();

	plane.vertex[0] = float3(width  / 2, -height / 2, 0);
	plane.vertex[1] = float3(width  / 2,  height / 2, 0);
	plane.vertex[2] = float3(-width / 2, -height / 2, 0);
	plane.vertex[3] = float3(-width / 2,  height / 2, 0);

	glBindBuffer(GL_ARRAY_BUFFER, plane.vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, plane.vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentImage::SaveComponent(Config &conf)
{
}

void ComponentImage::LoadComponent(Config & conf)
{
}

void ComponentImage::CreateImagePlane()
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

	glGenBuffers(1, (GLuint*)&plane.indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane.indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 6, plane.index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Texture * ComponentImage::CreateEmptyTexture()
{
	Texture* ret = new Texture(); 

	ret->height = 0;
	ret->width = 0;
	ret->path = "No Image Added";
	ret->texture_id = 0;
	ret->texture_name = "No Image Added";

	return ret;
}
