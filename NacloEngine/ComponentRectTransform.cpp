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
#include "Config.h"


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

float4x4 ComponentRectTransform::GetLocalCanvasMatrix()
{
	localmatrix = float4x4::FromTRS(position, quaternion, size);
	float4x4 ret = localmatrix;

	GameObject* tmp = container;

	if (tmp != nullptr && tmp->parent != nullptr && tmp->parent->canvas == nullptr) {

		while (tmp->parent != nullptr && tmp->parent->canvas == nullptr)
		{
			ComponentRectTransform* contParentTransform = (ComponentRectTransform*)tmp->parent->GetComponentByType(Component::COMPONENT_TYPE::COMPONENT_RECT_TRANSFORM);

			if (contParentTransform != nullptr) {

				float4x4 parentTransformMatrix = contParentTransform->localmatrix;

				ret = parentTransformMatrix * localmatrix;
			}
			else
				ret = localmatrix;

			tmp = tmp->parent;			
		}
	}
	else
		ret = localmatrix;

	return ret;
}



void ComponentRectTransform::SetPosition(float3 position)
{
	this->position = position;
	UpdateMatrix();
}

void ComponentRectTransform::SetRotation(float3 rotation)
{
	this->rotation = rotation;

	this->quaternion = Quat::FromEulerXYZ(this->rotation.x * DEGTORAD, this->rotation.y * DEGTORAD, this->rotation.z * DEGTORAD);
	UpdateMatrix();
}

void ComponentRectTransform::SetSize(float3 size)
{
	this->size = size;
	UpdateMatrix();
}

void ComponentRectTransform::SetQuaternion(Quat quaternion)
{
	this->rotation = quaternion.ToEulerXYZ();

	this->quaternion = quaternion;
	UpdateMatrix();
}

void ComponentRectTransform::SetPivot(float2 pivot)
{
	this->pivot = pivot;
}

void ComponentRectTransform::SetMinAnchor(float2 minAnchor)
{
	this->minAnchor = minAnchor;
}

void ComponentRectTransform::SetMaxAnchor(float2 maxAnchor)
{
	this->maxAnchor = maxAnchor;
}

void ComponentRectTransform::SetWidth(float width)
{
	this->width = width;
}

void ComponentRectTransform::SetHeight(float height)
{
	this->height = height;
}

float ComponentRectTransform::GetWidth()
{
	return width;
}

float ComponentRectTransform::GetHeight()
{
	return height;
}

void ComponentRectTransform::ShowInspector()
{
	if (App->engineState == ENGINE_STATE::EDITOR) {
		DrawGuizmos();
	}

	if (ImGui::CollapsingHeader("Rect Transform"))
	{
		float3 newPosition = position;
		float3 newSize = size;
		float2 newPivot = pivot;
		float2 newMinAnchor = minAnchor;
		float2 newMaxAnchor = maxAnchor;


		//POSITION------------
		ImGui::Text("Position");
		ImGui::SameLine();
		if (ImGui::DragFloat3("##pos", &newPosition[0], 0.1f)) {
			if (!container->staticGO)
				SetPosition(newPosition);
		}

		ImGui::Text("Size");
		ImGui::SameLine();
		if (ImGui::DragFloat3("##size", &newSize[0], 0.1f)) {
			if (!container->staticGO)
				SetSize(newSize);
		}


		ImGui::Text("Pivot");
		ImGui::SameLine();
		if (ImGui::DragFloat2("##pivot", &newPivot[0], 0.1f)) {
				SetPosition(newPosition);
		}

		ImGui::Text("minAnchor");
		ImGui::SameLine();
		if (ImGui::DragFloat2("##minAnchor", &newMinAnchor[0], 0.1f)) {
				SetMinAnchor(newMinAnchor);
		}

		ImGui::Text("maxAnchor");
		ImGui::SameLine();
		if (ImGui::DragFloat2("##maxAnchor", &newMaxAnchor[0], 0.1f)) {
			SetMaxAnchor(newMaxAnchor);
		}

		
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
			else if (container->parent->rectTransform != nullptr) {
				localmatrix = container->parent->rectTransform->globalMatrix.Inverted() * matrix;
				localmatrix.Decompose(position, quaternion, size);
				rotation = quaternion.ToEulerXYZ() * RADTODEG;
			}
			UpdateMatrix();
		}
	}
}

void ComponentRectTransform::Update(float dt)
{
	loading = false;
}

void ComponentRectTransform::SetNewSize(float width, float height)
{
	if (default_height != 0)
	{
		float new_size_y = height / default_height;
		SetSize(float3(size.x, new_size_y, size.z));
	}

	if (default_width != 0)
	{
		float new_size_x = width / default_width;
		SetSize(float3(new_size_x, size.y, size.z));
	}
}

void ComponentRectTransform::SaveComponent(Config &conf)
{
	conf.SetFloat3("Position", position);
	conf.SetFloat3("Size",	size);
	conf.SetFloat2("Pivot",	pivot);
	conf.SetFloat2("Pivot",	minAnchor);
	conf.SetFloat2("Pivot",	maxAnchor);
	conf.SetFloat("Width",	width);
	conf.SetFloat("Height",	height);


}

void ComponentRectTransform::LoadComponent(Config & conf)
{
	position	= conf.GetFloat3("Position");
	size		= conf.GetFloat3("Size");
	pivot		= conf.GetFloat2("Pivot");
	minAnchor	= conf.GetFloat2("Pivot");
	maxAnchor	= conf.GetFloat2("Pivot");
	width		= conf.GetFloat("Width");
	height		= conf.GetFloat("Height");
	loading = true;
}	
