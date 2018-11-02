#include "ComponentCamera.h"
#include "GameObject.h"
#include "ComponentTransform.h"

ComponentCamera::ComponentCamera(GameObject * container) : Component(container)
{
	type = COMPONENT_CAMERA;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(1.3 * tanf(frustum.verticalFov/2.0f));

}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update(float dt)
{
	ComponentTransform* go_transform = (ComponentTransform*)container->GetComponentByType(COMPONENT_TYPE::COMPONENT_TRANSFORM);

	float4x4 transformMatrix = go_transform->matrix;

	frustum.pos = transformMatrix.TranslatePart();
	frustum.front = transformMatrix.WorldZ();
	frustum.up = transformMatrix.WorldY();
}
