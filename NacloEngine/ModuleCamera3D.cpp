#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "Quadtree.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	meshBox = new AABB(float3(0.0f,0.0f,0.0f), float3(0.0f, 0.0f, 0.0f));
	empty_meshBox = new AABB(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f));
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	speed = 3.0f;
	aux_speed = 3.0f;
	fast_speed = 8.0f;
	scroll_speed = 8.0f;

	// Rotating the frustrum to look at the scene correctly
	/*
	float3x3 rotationMatrix = float3x3::RotateAxisAngle(camera->frustum.WorldRight().Normalized(), 131 * DEGTORAD);
	camera->frustum.up = rotationMatrix * camera->frustum.up;
	camera->frustum.front = rotationMatrix * camera->frustum.front;
	*/
	camera = new ComponentCamera(nullptr);
	
	//camera->frustum.Translate(float3(3.0f, 5.0f, 3.0f));
	camera->frustumCulling = false;
	camera->frustum.Translate(float3(5, 10, 5));
	LookAt(float3(0.0f, 0.0f, 0.0f));

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	delete meshBox;
	delete empty_meshBox;
	RELEASE(camera);

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (!ImGui::IsMouseHoveringAnyWindow())
		{
			float winWidth = (float)App->window->width;
			float winHeight = (float)App->window->height;

			int mouse_x = App->input->GetMouseX();
			int mouse_y = App->input->GetMouseY();

			float normalized_x = -(1.0f - (float(mouse_x) * 2.0f) / winWidth);
			float normalized_y = 1.0f - (float(mouse_y) * 2.0f) / winHeight;

			LineSegment ray = camera->frustum.UnProjectLineSegment(normalized_x, normalized_y);		
			MousePick(posible_go_intersections, ray);
			debugRay = ray;
		}
	}
	RaycastDebugDraw();
	
	Move(dt);

	//-----------------------------Focus mesh-----------------------------
	/*
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
		LookAtMeshBox();
	}
	*/

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			Orbit(dx*dt, dy*dt);
		else
			Look(dx*dt, dy*dt);
	}

	return UPDATE_CONTINUE;
}
// -----------------------------------------------------------------


void ModuleCamera3D::Orbit(float dx, float dy)
{
	float3 point = looking_at;

	// fake point should be a ray colliding with something
	if (looking == false)
	{
		point = camera->frustum.pos + camera->frustum.front * 50.0f;

		looking = true;
		looking_at = point;
	}

	float3 focus = camera->frustum.pos - point;

	Quat qy(camera->frustum.up, dx*0.2);
	Quat qx(camera->frustum.WorldRight(), dy*0.2);

	focus = qx.Transform(focus);
	focus = qy.Transform(focus);

	camera->frustum.pos = focus + point;

	LookAt(point);
}


// -----------------------------------------------------------------

void ModuleCamera3D::Look(float dx, float dy)
{
	if (dx != 0.f)
	{
		Quat q = Quat::RotateY(dx*0.2);
		camera->frustum.front = q.Mul(camera->frustum.front).Normalized();
		camera->frustum.up = q.Mul(camera->frustum.up).Normalized();
	}

	if (dy != 0.f)
	{
		Quat q = Quat::RotateAxisAngle(camera->frustum.WorldRight(), dy * 0.2);
		float3 new_up = q.Mul(camera->frustum.up).Normalized();
		if (new_up.y > 0.0f)
		{
			camera->frustum.up = new_up;
			camera->frustum.front = q.Mul(camera->frustum.front).Normalized();
		}
	}
}

void ModuleCamera3D::LookAt(const float3 & position)
{
	float3 dir = position - camera->frustum.pos;

	float3x3 m = float3x3::LookAt(camera->frustum.front, dir.Normalized(), camera->frustum.up, float3::unitY);

	camera->frustum.front = m.MulDir(camera->frustum.front).Normalized();
	camera->frustum.up = m.MulDir(camera->frustum.up).Normalized();
}

// -----------------------------------------------------------------

void ModuleCamera3D::Move(float dt)
{
	float speed = 3.0f * dt;
	float3 right(camera->frustum.WorldRight());
	float3 forward(camera->frustum.front);
	float3 newPos = float3::zero;

	//-----------------------------Speed boost-----------------------------
	speed = aux_speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = fast_speed * dt;


	//-----------------------------ARROWS movement-----------------------------
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) newPos += forward;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) newPos -= forward;
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) newPos -= right;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) newPos += right;

	//-----------------------------WASD movement-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += forward;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= forward;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= right;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += right;
	}

	//-----------------------------Wheel mouse zoom-----------------------------
	if (App->input->GetMouseZ() == 1)
		newPos += forward * scroll_speed;

	if (App->input->GetMouseZ() == -1)
		newPos -= forward * scroll_speed;

	//-----------------------------Wheel mouse movement-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		if (App->input->GetMouseXMotion() > 0)
			newPos -= right * (App->input->GetMouseXMotion() * dt) * speed * 15;

		if (App->input->GetMouseXMotion() < 0)
			newPos -= right * (App->input->GetMouseXMotion() * dt) * speed * 15;

		if (App->input->GetMouseYMotion() > 0)
			newPos += camera->frustum.up * (App->input->GetMouseYMotion() * dt) * speed * 15;

		if (App->input->GetMouseYMotion() < 0)
			newPos += camera->frustum.up * (App->input->GetMouseYMotion() * dt) * speed * 15;
	}
	   
	if (newPos.Equals(float3::zero) == false)
	{
		camera->frustum.Translate(newPos);
		looking = false;
	}
}


// -----------------------------------------------------------------
void ModuleCamera3D::CreateMeshBox(float3 minVertex, float3 maxVertex)
{
	meshBox->minPoint = minVertex;
	meshBox->maxPoint = maxVertex;
}

void ModuleCamera3D::LookAtMeshBox()
{
	
}

void ModuleCamera3D::ShowCameraInfo()
{
	ImGui::InputFloat("Camera Speed", &aux_speed, 1.0f);
	ImGui::InputFloat("Fast Camera Speed", &fast_speed, 1.0f);
	ImGui::InputFloat("Scroll Camera Speed", &scroll_speed, 1.0f);

}

void ModuleCamera3D::CullingGameObjects(GameObject * go)
{
	if (App->scene->main_camera->camera->frustumCulling) {
		if (!App->scene->main_camera->camera->Intersects(go->boundingBox)) {
			go->active = false;
		}
		else
			go->active = true;
	}
	for (uint i = 0; i < go->children.size(); ++i)
	{
		CullingGameObjects(go->children[i]);
	}
}

void ModuleCamera3D::MousePick(std::vector<GameObject*> &candidates, LineSegment ray)
{
	candidates.clear();

	App->scene->quadtree->Intersect(candidates, ray);

	for (uint i = 0; i < App->scene->root->children.size(); ++i)
	{
		PickCandidates(candidates, App->scene->root->children[i]);
	}

	float hit_distance = 0.0F;

	Mesh* mesh = nullptr;

	GameObject* selected_object = nullptr;

	for (std::vector<GameObject*>::const_iterator it = candidates.begin(); it != candidates.end(); ++it)
	{
		Triangle tri;
		LineSegment localRay(ray);

		ComponentTransform* transform = (*it)->transform;

		if (transform != nullptr) {

			localRay.Transform(transform->globalMatrix.Inverted());

			ComponentMesh* cMesh = (*it)->mesh;
			mesh = cMesh->mesh;

			if (cMesh != nullptr) {

				localRay.Transform(transform->globalMatrix.Inverted());
				mesh = cMesh->mesh;

				for (uint i = 0; i < mesh->num_indices;)
				{
					tri.a = mesh->vertices[mesh->indices[i++] * 3];
					tri.b = mesh->vertices[mesh->indices[i++] * 3];
					tri.c = mesh->vertices[mesh->indices[i++] * 3];

					float distance;
					float3 hit_point;
					if (localRay.Intersects(tri, &distance, &hit_point))
					{
						selected_object = (*it);
					}
				}
			}
		}
	}
	if (selected_object != nullptr)
	{
		App->scene->selected = selected_object;
	}
}

void ModuleCamera3D::PickCandidates(std::vector<GameObject*> &pick_candidates, GameObject* candidate)
{

	if (!camera->Intersects(candidate->boundingBox))
	{
		pick_candidates.push_back(candidate);
	}


	for (uint i = 0; i < candidate->GetNumChildren(); ++i)
	{
		PickCandidates(pick_candidates, candidate->children[i]);
	}


}

void ModuleCamera3D::RaycastDebugDraw()
{	
	glBegin(GL_LINES);
	glLineWidth(50.0f);
	glColor4f(0.25f, 1.0f, 0.0f, 1.0f);

	glVertex3f(debugRay.a.x, debugRay.a.y, debugRay.a.z);
	glVertex3f(debugRay.b.x, debugRay.b.y, debugRay.b.z);

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

// -----------------------------------------------------------------

