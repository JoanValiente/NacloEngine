#include "ComponentTransform.h"
#include "GameObject.h"
#include "Globals.h"

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

	matrix = float4x4::FromTRS(position, quaternion, size);

	if (container != nullptr && container->parent != nullptr) {

		ComponentTransform* contParentTransform = (ComponentTransform*)container->parent->GetComponentByType(Component::COMPONENT_TYPE::COMPONENT_TRANSFORM);

		if (contParentTransform != nullptr) {

			float4x4 parentTransformMatrix = contParentTransform->globalMatrix;

			globalMatrix = parentTransformMatrix * matrix;
		}
		else
			globalMatrix = matrix;
	}
	else
		globalMatrix = matrix;

	if (!globalMatrix.Equals(prevGlobal))
		if (container != nullptr)
			container->UpdateBoundingBox();
}

void ComponentTransform::ShowInspector()
{
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


