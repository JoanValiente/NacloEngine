#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib/Geometry/Frustum.h"

class ComponentCamera : public Component {

public:
	ComponentCamera(GameObject* container);
	~ComponentCamera();

	void Update(float dt);
	

public:
	Frustum frustum;
};

#endif
