#include "Component.h"
#include "ComponentRectTransform.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"


ComponentRectTransform::ComponentRectTransform(GameObject* container) : Component(container)
{
	this->type = COMPONENT_RECT_TRANSFORM;

	this->pivot.x = 0.5;
	this->pivot.y = 0.5;

	this->minAnchor.x = 0.5;
	this->minAnchor.y = 0.5;

	this->maxAnchor.x = 0.5;
	this->maxAnchor.y = 0.5;

	UpdateMatrix();
}

void ComponentRectTransform::UpdateMatrix()
{
	localmatrix = float4x4::FromTRS(position, quaternion, size);

	if (container != nullptr && container->parent != nullptr) {

		ComponentRectTransform* contParentTransform = (ComponentRectTransform*)container->parent->GetComponentByType(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);

		if (contParentTransform != nullptr) {

			float4x4 parentTransformMatrix = contParentTransform->globalMatrix;

			globalMatrix = parentTransformMatrix * localmatrix;
		}
		else
			globalMatrix = localmatrix;
	}
	else
		globalMatrix = localmatrix;
}



void ComponentRectTransform::SetPosition(float3 position)
{
	this->position = position;
	UpdateMatrix();
}


void ComponentRectTransform::ShowInspector()
{
	if (App->engineState == ENGINE_STATE::EDITOR) {
		DrawGuizmos();
	}

	if (ImGui::CollapsingHeader("Rect Transform"))
	{
		float3 newPosition = position;
		//float3 newRotation = rotation;
		//float3 newSize = size;

		//POSITION------------
		ImGui::Text("Position");
		ImGui::SameLine();
		if (ImGui::DragFloat3("##pos", &newPosition[0], 0.1f)) {
			if (!container->staticGO)
				SetPosition(newPosition);
		}

		/*
		float newWidth = width;
		ImGui::Text("Width");
		ImGui::SameLine();
		if (ImGui::DragFloat("##width", &newWidth, 0.1f)) {
			if (!container->staticGO)
				SetWidth(newWidth);
		}
		float newHeight = height;
		ImGui::Text("Height");
		ImGui::SameLine();
		if (ImGui::DragFloat("##height", &newHeight, 0.1f)) {
			if (!container->staticGO)
				SetHeight(newHeight);
		}
		*/

		/*
		//ROTATION------------
		ImGui::Text("Rotation");
		ImGui::SameLine();
		if (ImGui::DragFloat3("##rot", &newRotation[0], 0.1f)) {
			if (!container->staticGO)
				SetRotation(newRotation);
		}

		//ROTATION------------
		ImGui::Text("Size");
		ImGui::SameLine();
		if (ImGui::DragFloat3("##size", &newSize[0], 0.1f)) {
			if (!container->staticGO)
				SetSize(newSize);
		}
		*/

	}
}

void ComponentRectTransform::DrawGuizmos()
{
	ImGuizmo::Enable(true);

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	static ImGuizmo::OPERATION guizmoOperation = ImGuizmo::TRANSLATE;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) guizmoOperation = ImGuizmo::TRANSLATE;

	//if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) guizmoOperation = ImGuizmo::ROTATE;

	//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)	guizmoOperation = ImGuizmo::SCALE;

	math::float4x4 matrix = globalMatrix.Transposed();

	float4x4 cameraProjMat;
	cameraProjMat = App->camera->camera->frustum.ProjectionMatrix();
	cameraProjMat.Transpose();

	ImGuizmo::Manipulate(App->camera->camera->GetViewMatrix(), (float*)cameraProjMat.v, guizmoOperation, ImGuizmo::WORLD, matrix.ptr());

	if (ImGuizmo::IsUsing() && !container->staticGO)
	{
		matrix.Transpose();

		App->camera->using_guizmos = true;

		if (guizmoOperation == ImGuizmo::SCALE) {

			math::Quat tmp;
			matrix.Decompose(position, tmp, size);
			UpdateMatrix();
		}

		else {
			if (container->parent == nullptr)
			{
				localmatrix = matrix;
			}
			else {
				localmatrix = container->parent->rectTransform->globalMatrix.Inverted() * matrix;
				localmatrix.Decompose(position, quaternion, size);
				rotation = quaternion.ToEulerXYZ() * RADTODEG;
			}
			UpdateMatrix();
		}
	}
}

void ComponentRectTransform::SaveComponent(Config &conf)
{
}

void ComponentRectTransform::LoadComponent(Config & conf)
{
}