#ifndef __ModuleCamera3D_H__
#define __ModuleCamera3D_H__

#include "Module.h"
#include "MathGeoLib/MathGeoLib.h"
//#include "glmath.h"

struct ComponentCamera;
struct GameObject;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	
	void MoveArroundReference(float dx, float dy);
	void Look(float dx, float dy);
	void LookAt(const float3& position);
	void Move(float dt);
	
	void CreateMeshBox(float3 minVertex, float3 maxVertex);
	void LookAtMeshBox();
	void ShowCameraInfo();

	//void CullingGameObjects(GameObject* go);
	void MousePick(std::vector<GameObject*> &candidates, LineSegment ray);
	void RaycastDebugDraw();
	void PickCandidates(std::vector<GameObject*> &pick_candidates, GameObject* candidate);

public:
	
	AABB* meshBox = nullptr;
	AABB* empty_meshBox = nullptr;
	LineSegment debugRay;

	ComponentCamera* camera;
	ComponentCamera* activeCamera;

	bool using_guizmos = false;
private:
	float speed;
	float aux_speed;
	float fast_speed;
	float scroll_speed;
	bool looking = false;
	float3 looking_at;
	std::vector<GameObject*> posible_go_intersections;
};

#endif