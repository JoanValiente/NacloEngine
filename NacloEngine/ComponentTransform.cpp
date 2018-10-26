#include "ComponentTransform.h"
#include "Globals.h"

ComponentTransform::ComponentTransform(GameObject * parent, float3 position, float3 rotation, float3 size) : Component(parent)
{
	this->position = position;
	this->rotation = rotation;
	this->size = size;

	type = COMPONENT_TRANSFORM;

	quaternion = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD);

	matrix = float4x4::FromTRS(position, quaternion, size);
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::UpdateMatrix(float3 position, Quat quaternion, float3 size)
{
	matrix = float4x4::FromTRS(position, quaternion, size);
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


