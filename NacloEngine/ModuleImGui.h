#pragma once
#include "Module.h"
//#include "p2DynArray.h"
//#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "ImGui/imgui_impl_sdl.h"
#include "SDL/include/SDL_opengl.h"
#include "PCG\pcg_variants.h"
#include "Primitive.h"
#include "PCG/pcg_variants.h"
#include "PCG/entropy.h"


#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleImgui : public Module
{
public:
	ModuleImgui(Application* app, bool start_enabled = true);
	~ModuleImgui();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	Plane* mPlane;
	Sphere* mSphere;
	Capsule* mCapsule;
	AABB* mAabb;
	Frustum* mFrustum;
	Ray* mRay;
	Triangle* mTriangle;

private:
	bool show_demo_window = true;
	float clear_color = 0.5f;
	pcg32_random_t rng;
	bool show_random_window = false;
	bool show_intersection_window = false;

};
