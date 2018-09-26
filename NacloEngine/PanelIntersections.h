#ifndef __PANELINTERSECTIONS_H__
#define __PANELINTERSECTIONS_H__

#include "Panel.h"
#include "Primitive.h"

class PanelIntersections : public Panel
{
public:
	PanelIntersections();
	virtual ~PanelIntersections();

	void Draw();

private:
	Plane* mPlane;
	Sphere* mSphere;
	Capsule* mCapsule;
	AABB* mAabb;
	Frustum* mFrustum;
	Ray* mRay;
	Triangle* mTriangle;
};

#endif
