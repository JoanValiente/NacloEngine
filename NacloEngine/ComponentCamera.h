#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib/Geometry/Frustum.h"

class ComponentCamera : public Component {

public:
	ComponentCamera(GameObject* container);
	~ComponentCamera();

	void ShowInspector();

	void Update(float dt);
	float* GetViewMatrix();
	void DebugDraw();
	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);
	
	bool Intersects(const AABB box);


private:
	void SetFar(float frustum_far);
	void SetNear(float frustum_near);
	void SetFov(float frustum_fov);
	float GetFov() const;
	void SetAspectRatio(float f_ratio);

public:
	Frustum frustum;
	bool frustumCulling = false;
	bool debugDraw = false;
};

#endif
