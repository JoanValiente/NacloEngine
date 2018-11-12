#include "ComponentCamera.h"
#include "Glew/include/glew.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Component.h"

ComponentCamera::ComponentCamera(GameObject * container) : Component(container)
{
	type = COMPONENT_CAMERA;

	frustum.type = PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(1.3 * tanf(frustum.verticalFov/2.0f));

	debugDraw = true;
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update(float dt)
{
	ComponentTransform* go_transform = (ComponentTransform*)container->GetComponentByType(COMPONENT_TYPE::COMPONENT_TRANSFORM);

	float4x4 transformMatrix = float4x4::identity;

	if (go_transform != nullptr) {
		transformMatrix = go_transform->globalMatrix;
	}
	frustum.pos = transformMatrix.TranslatePart();
	frustum.front = transformMatrix.WorldZ();
	frustum.up = transformMatrix.WorldY();

	if (debugDraw) {
		DebugDraw();
	}
}

float* ComponentCamera::GetViewMatrix()
{
	static float4x4 m;
	m = frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.v;
}

void ComponentCamera::DebugDraw()
{

	glBegin(GL_LINES);
	glLineWidth(3.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
	}
	glEnd();

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

}

bool ComponentCamera::Intersects(const AABB box)
{
	bool ret = true;

	float3 vCorner[8];
	int iTotalIn = 0;
	box.GetCornerPoints(vCorner); 

	for (int p = 0; p < 6; ++p) {

		int iInCount = 8;
		int iPtIn = 1;

		for (int i = 0; i < 8; ++i) {
			
			if (frustum.GetPlane(p).IsOnPositiveSide(vCorner[i])) {
				iPtIn = 0;
				--iInCount;
			}
		}

		if(iInCount == 0)
			return false;

		iTotalIn += iPtIn;
	}
	
	return ret;
}
