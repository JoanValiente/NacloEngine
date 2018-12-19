#include "OpenGL.h"
#include "Component.h"
#include "ComponentImage.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"
#include "ComponentRectTransform.h"


ComponentImage::ComponentImage(GameObject* container) : Component(container)
{
	this->type = COMPONENT_IMAGE;

	if (container->rectTransform != nullptr)
	{
		container->rectTransform->SetHeight(100.0f);
		container->rectTransform->SetWidth(100.0f);

		CreateImagePlane();
		container->rectTransform->UpdateMatrix();
	}

	else
	{
		LOG("Error creating Image Rect, no rect transform component created");
	}
}

void ComponentImage::ShowInspector()
{	
	if (ImGui::CollapsingHeader("Image"))
	{
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

	glColor3f(255, 255, 255);

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

void ComponentImage::UpdateImagePlane()
{
	plane.vertex[0] = float3(container->rectTransform->GetWidth(), 0, 0);
	plane.vertex[1] = float3(container->rectTransform->GetWidth(), container->rectTransform->GetHeight(), 0);
	plane.vertex[2] = float3(0, 0, 0);
	plane.vertex[3] = float3(0, container->rectTransform->GetHeight(), 0);

	glBindBuffer(GL_ARRAY_BUFFER, plane.vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, plane.vertex, GL_STATIC_DRAW);
}

void ComponentImage::SaveComponent(Config &conf)
{
}

void ComponentImage::LoadComponent(Config & conf)
{
}

void ComponentImage::CreateImagePlane()
{
	plane.vertex[0] = float3(container->rectTransform->GetWidth(), 0, 0);
	plane.uv[0] = float2(1, 0);

	plane.vertex[1] = float3(container->rectTransform->GetWidth(), container->rectTransform->GetHeight(), 0);
	plane.uv[1] = float2(1, 1);

	plane.vertex[2] = float3(0, 0, 0);
	plane.uv[2] = float2(0, 0);

	plane.vertex[3] = float3(0, container->rectTransform->GetHeight(), 0);
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
