#include "ComponentTransform.h"
#include "GameObject.h"
#include "Globals.h"

ComponentTransform::ComponentTransform(GameObject * container) : Component(container)
{
	type = COMPONENT_TRANSFORM;
	active = true;

	quaternion = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD);

	matrix = float4x4::FromTRS(position, quaternion, size);
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

			float4x4 parentTransformMatrix = contParentTransform->matrix;

			globalMatrix = parentTransformMatrix * matrix;
		}
		else
			globalMatrix = matrix;
	}

	if (!globalMatrix.Equals(prevGlobal))
		if (container != nullptr)
			container->UpdateBoundingBox();
}

void ComponentTransform::SetPosition(float3 position)
{
	this->position = position;

	UpdateMatrix(this->position, this->quaternion, this->size);
}

void ComponentTransform::SetRotation(float3 rotation)
{
	this->rotation = rotation;

	this->quaternion = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD);

	UpdateMatrix(this->position, this->quaternion, this->size);
}

void ComponentTransform::SetSize(float3 size)
{
	this->size = size;

	UpdateMatrix(this->position, this->quaternion, this->size);
}

void ComponentTransform::SetQuaternion(Quat quaternion)
{
	this->rotation = quaternion.ToEulerXYZ();

	this->quaternion = quaternion;

	UpdateMatrix(this->position, this->quaternion, this->size);
}


