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
#include "ComponentCanvas.h"
#include "Config.h"

ComponentImage::ComponentImage(GameObject* container) : ComponentInteractive(container)
{
	this->type = COMPONENT_IMAGE;


	if (container->rectTransform != nullptr)
	{
		if (!container->rectTransform->loading)
		{
			container->rectTransform->SetHeight(size.x);
			container->rectTransform->SetWidth(size.y);
		}

		CreateImagePlane();
		container->rectTransform->UpdateMatrix();

		tex = CreateEmptyTexture();
	}

	else
	{
		LOG("Error creating Image Rect, no rect transform component created");
	}

	if (container->parent != nullptr)
	{
		AddToTheList();
	}
}

ComponentImage::~ComponentImage()
{
	delete tex;
}

void ComponentImage::Update(float dt)
{
	if (!added)
	{
		AddToTheList();
	}
	if (preserveAspect)
	{
		PreserveAspect();
	}
}

void ComponentImage::ShowInspector()
{
	if (ImGui::CollapsingHeader("Image"))
	{
		if (tex->texture_id != 0)
		{
			ImGui::InputText("##Source Image", (char*)tex->texture_name.c_str(), 64);
			ImGui::SameLine();
			ImGui::Image((ImTextureID)tex->texture_id, { 20,20 });
			ImGui::SameLine();

			if (ImGui::ArrowButton("Change Texture", ImGuiDir_Down))
			{
				ImGui::OpenPopup("Change_Texture");
			}

			ImGui::SliderFloat("Alpha", &color.w, 0.0f, 1.0f);

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

			ImGui::ColorEdit4("Color##image_rgba", color.ptr());
		}

		if (ImGui::Checkbox("Dragable", &dragable))
		{
			interactive = !interactive;
		}
		if (tex->texture_id != 0)
		{
			if (ImGui::Checkbox("Preserve Aspect", &preserveAspect))
			{
				aux_width = container->rectTransform->width;
				aux_height = container->rectTransform->height;

				if (preserveAspect)
				{
					float ratio = tex->width / tex->height;
					container->rectTransform->height = container->rectTransform->width / ratio;
					container->rectTransform->width = container->rectTransform->height * ratio;
				}
			}

			if (ImGui::Button("Set Native Size"))
			{
				container->rectTransform->SetWidth(tex->width);
				container->rectTransform->SetHeight(tex->height);
			}
		}

		else
		{
			ImGui::Text("None (Texture)");
			ImGui::SameLine();

			if (ImGui::ArrowButton("Add Texture", ImGuiDir_Down))
			{
				ImGui::OpenPopup("Change_Texture");
			}

			ImGui::ColorEdit4("Color##image_rgba", color.ptr());

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
	}
}

void ComponentImage::AssignTexture(Texture * texture)
{
	this->tex = texture;
}

void ComponentImage::Render(uint texture_id)
{
	if (container->active)
	{
		glPushMatrix();
		UpdateImagePlane();
		float4x4 matrix = container->rectTransform->globalMatrix;
		glMultMatrixf((GLfloat*)matrix.Transposed().ptr());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);

		glColor4f(color.x, color.y, color.z, color.w);

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

void ComponentImage::PreserveAspect()
{
	float ratio = tex->width / tex->height;
	float ratio2 = container->rectTransform->width / container->rectTransform->height;
	if (ratio != ratio2 )
	{
		if (aux_width != container->rectTransform->width)
		{
			container->rectTransform->height = container->rectTransform->width / ratio;
			aux_width = container->rectTransform->width;
			aux_height = container->rectTransform->height;
		}
		else if (aux_height != container->rectTransform->height)
		{
			container->rectTransform->width = container->rectTransform->height * ratio;
			aux_width = container->rectTransform->width;
			aux_height = container->rectTransform->height;
		}
	}
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

void ComponentImage::SaveComponent(Config &conf)
{
	if (tex != nullptr)
	{
		conf.SetString("Path", tex->path.c_str());
		conf.SetString("Name", tex->texture_name.c_str());
		conf.SetString("DDS Path", tex->texture_dds.c_str());
		conf.SetInt("Width", tex->width);
		conf.SetInt("Height", tex->height);
	}

	conf.SetFloat4("Color", color);
	conf.SetBool("Preserve Aspect", preserveAspect);
	conf.SetBool("Dragable", dragable);
}

void ComponentImage::LoadComponent(Config & conf)
{
	if (conf.GetString("DDS Path") != NULL)
	{
		tex = App->texture->LoadTexture(conf.GetString("DDS Path"));
		if (tex != nullptr)
		{
			tex->texture_dds = conf.GetString("DDS Path");
			tex->path = conf.GetString("Path");
			tex->texture_name = conf.GetString("Name");
			tex->width = conf.GetInt("Width");
			tex->height = conf.GetInt("Height");
		}
	}

	color = conf.GetFloat4("Color");
	preserveAspect = conf.GetBool("Preserve Aspect");
	dragable = conf.GetBool("Dragable");
}

bool ComponentImage::Fade(float dt)
{
	if (color.w <= 0.01)
	{
		container->active = false;
		return true;
	}
	else
	{
		color.w -= 2 * dt;
		return false;
	}
}

