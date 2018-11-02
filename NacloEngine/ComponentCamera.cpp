#include "ComponentCamera.h"
#include "GameObject.h"
#include "ComponentTransform.h"

ComponentCamera::ComponentCamera(GameObject * container) : Component(container)
{
	type = COMPONENT_CAMERA;

	frustum.type = math::FrustumType::PerspectiveFrustum;
	frustum.pos = math::float3::zero;
	frustum.front = math::float3::unitZ;
	frustum.up = math::float3::unitY;
	frustum.nearPlaneDistance = 10.0F;
	frustum.farPlaneDistance = 1000.0F;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(1.3 * tanf(frustum.verticalFov/2.0f));

}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update(float dt)
{
	ComponentTransform* go_transform = (ComponentTransform*)container->GetComponentByType(COMPONENT_TYPE::COMPONENT_TRANSFORM);

	math::float4x4 transformMatrix = go_transform->matrix;

	frustum.pos = transformMatrix.TranslatePart();
	frustum.front = transformMatrix.WorldZ();
	frustum.up = transformMatrix.WorldY();
}
