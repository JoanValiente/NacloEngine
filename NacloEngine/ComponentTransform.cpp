#include "ComponentTransform.h"
#include "GameObject.h"
#include "Globals.h"
#include "ImGuizmo/ImGuizmo.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ComponentCamera.h"
#include "Config.h"

ComponentTransform::ComponentTransform(GameObject * container) : Component(container)
{
	type = COMPONENT_TRANSFORM;
	active = true;

	quaternion = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD);

	UpdateMatrix(position, quaternion, size);
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::UpdateMatrix(float3 position, Quat quaternion, float3 size)
{
	float4x4 prevGlobal = globalMatrix;

	localmatrix = float4x4::FromTRS(position, quaternion, size);

	if (container != nullptr && container->parent != nullptr) {

		ComponentTransform* contParentTransform = (ComponentTransform*)container->parent->GetComponentByType(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);

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

void ComponentTransform::ShowInspector()
{
	DrawGuizmos();

	if (ImGui::CollapsingHeader("Transformation"))
	{
		//POSITION------------
		ImGui::Text("Position");
		ImGui::Text("X:"); ImGui::SameLine();
		if (ImGui::DragFloat("##pos_x", &position.x, 0.1f))
			uint patata = 0;

		ImGui::Text("Y:");	ImGui::SameLine();
		ImGui::DragFloat("##pos_y", &position.y, 0.1f); 
		
		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::DragFloat("##pos_z", &position.z, 0.1f);

		SetPosition(position);

		//ROTATION------------
		ImGui::Text("Rotation");
		ImGui::Text("X:");	ImGui::SameLine();
		ImGui::DragFloat("##rot_x", &rotation.x, 0.1f);

		ImGui::Text("Y:"); ImGui::SameLine();
		ImGui::DragFloat("##rot_y", &rotation.y, 0.1f);

		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::DragFloat("##rot_z", &rotation.z, 0.1f);

		SetRotation(rotation);

		//SCALE------------
		ImGui::Text("Scale");
		ImGui::Text("X:");ImGui::SameLine();
		ImGui::DragFloat("##size_x", &size.x, 0.1f);

		ImGui::Text("Y:");ImGui::SameLine();
		ImGui::DragFloat("##size_y", &size.y, 0.1f);

		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::DragFloat("##size_z", &size.z, 0.1f);

		SetSize(size);
	}
}

void ComponentTransform::SetPosition(float3 position)
{
	this->position = position;
}

void ComponentTransform::SetRotation(float3 rotation)
{
	this->rotation = rotation;

	this->quaternion = Quat::FromEulerXYZ(this->rotation.x * DEGTORAD, this->rotation.y * DEGTORAD, this->rotation.z * DEGTORAD);
}

void ComponentTransform::SetSize(float3 size)
{
	
	this->size = size;
}

void ComponentTransform::SetQuaternion(Quat quaternion)
{
	this->rotation = quaternion.ToEulerXYZ();

	this->quaternion = quaternion;

	UpdateMatrix(this->position, this->quaternion, this->size);
}

float4x4 ComponentTransform::GetLocalMatrix()
{
	localmatrix = float4x4::FromTRS(position, quaternion, size);
	return localmatrix;
}

float4x4 ComponentTransform::GetGlobalMatrix()
{
	return globalMatrix;
}

void ComponentTransform::SetGlobalMatrix(float4x4 matrix)
{
	globalMatrix = matrix;
}

void ComponentTransform::DrawGuizmos()
{
	ImGuizmo::Enable(true);

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(position.x, position.y, io.DisplaySize.x, io.DisplaySize.y);

	static ImGuizmo::OPERATION guizmoOperation = ImGuizmo::TRANSLATE;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) guizmoOperation = ImGuizmo::TRANSLATE;

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) guizmoOperation = ImGuizmo::ROTATE;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)	guizmoOperation = ImGuizmo::SCALE;
	
	math::float4x4 matrix = globalMatrix.Transposed();

	float4x4 cameraProjMat;
	cameraProjMat = App->camera->camera->frustum.ProjectionMatrix();
	cameraProjMat.Transpose();	

	ImGuizmo::Manipulate(App->camera->camera->GetViewMatrix(), (float*)cameraProjMat.v, guizmoOperation, ImGuizmo::WORLD, matrix.ptr());

	matrix.Transpose();

	if (ImGuizmo::IsUsing())
	{
		container->staticGO = false;
		App->camera->using_guizmos = true;

		if (container->parent == nullptr)
		{
			localmatrix = matrix;
		}
		else
		{

			localmatrix = container->parent->transform->GetGlobalMatrix().Inverted() * matrix;
		}

		localmatrix.Decompose(position, quaternion, size);
		rotation = quaternion.ToEulerXYZ() * RADTODEG;
		UpdateMatrix(position, quaternion, size);
	}

}

void ComponentTransform::SaveComponent(Config & conf)
{
	conf.SetFloat3("Position", position);
}



