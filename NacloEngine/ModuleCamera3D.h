#ifndef __ModuleCamera3D_H__
#define __ModuleCamera3D_H__

#include "Module.h"
#include "MathGeoLib/MathGeoLib.h"
//#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	
	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	
	float* GetViewMatrix();
	
	void CreateMeshBox(float3 minVertex, float3 maxVertex);
	void LookAtMeshBox();

private:

	void CalculateViewMatrix();

public:
	
	float3 X, Y, Z, Position, Reference;
	AABB* meshBox = nullptr;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
};

#endif