#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "GameObject.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	Position = float3(0.0f, 0.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	
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
	//goCamera = new GameObject(App->scene->root, "Main Camera");

	camera = new ComponentCamera(nullptr);
	
	//camera->frustum.Translate(float3(3.0f, 5.0f, 3.0f));
	Move(float3(3.0f, 5.0f, 3.0f));
	LookAt(float3(0.0f, 0.0f, 0.0f));

	CalculateViewMatrix();

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	delete meshBox;
	delete empty_meshBox;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos(0, 0, 0);

	float speed = 3.0f * dt;

	//-----------------------------Speed boost-----------------------------
	speed = aux_speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = fast_speed * dt;


	//-----------------------------ARROWS movement-----------------------------
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) newPos -= camera->frustum.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) newPos += camera->frustum.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) newPos -= camera->frustum.WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) newPos += camera->frustum.WorldRight() * speed;

	camera->frustum.Translate(newPos);

	//-----------------------------WASD movement-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= camera->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += camera->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= camera->frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += camera->frustum.WorldRight() * speed;

		camera->frustum.Translate(newPos);
	}

	//-----------------------------Wheel mouse zoom-----------------------------
	if (App->input->GetMouseZ() == 1)
		newPos -= camera->frustum.front * scroll_speed;

	if (App->input->GetMouseZ() == -1)
		newPos += camera->frustum.front * scroll_speed;

	camera->frustum.Translate(newPos);

	//-----------------------------Wheel mouse movement-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		if (App->input->GetMouseXMotion() > 0)
			newPos -= camera->frustum.WorldRight() * (App->input->GetMouseXMotion() * dt) * speed * 15;

		if (App->input->GetMouseXMotion() < 0)
			newPos -= camera->frustum.WorldRight() * (App->input->GetMouseXMotion() * dt) * speed * 15;

		if (App->input->GetMouseYMotion() > 0)
			newPos += camera->frustum.up * (App->input->GetMouseYMotion() * dt) * speed * 15;

		if (App->input->GetMouseYMotion() < 0)
			newPos += camera->frustum.up * (App->input->GetMouseYMotion() * dt) * speed * 15;

		camera->frustum.Translate(newPos);
	}

	Position += newPos;
	Reference += newPos;
	

	//-----------------------------Focus mesh-----------------------------
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
		LookAtMeshBox();
	}

	//-----------------------------LEFT Mouse motion-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		float Sensitivity = 0.01f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3x3 rotationMatrix = float3x3::RotateY(DeltaX);
			camera->frustum.WorldRight() = rotationMatrix * camera->frustum.WorldRight();
			camera->frustum.up = rotationMatrix * camera->frustum.up;
			camera->frustum.front = rotationMatrix * camera->frustum.front;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			float3x3 rotationMatrix = float3x3::RotateAxisAngle(camera->frustum.WorldRight().Normalized(), DeltaY);
			camera->frustum.up = rotationMatrix * camera->frustum.up;
			camera->frustum.front = rotationMatrix * camera->frustum.front;

			if (camera->frustum.up.y < 0.0f)
			{
				camera->frustum.front = float3(0.0f, camera->frustum.front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				camera->frustum.up = Cross(camera->frustum.front, camera->frustum.WorldRight());
			}
		}

		Position = Reference + camera->frustum.front * Position.Length();
	}
	//-----------------------------RIGHT Mouse motion-----------------------------
	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		float Sensitivity = 0.005f;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3x3 rotationMatrix = float3x3::RotateY(DeltaX);
			camera->frustum.WorldRight() = rotationMatrix * camera->frustum.WorldRight();
			camera->frustum.up = rotationMatrix * camera->frustum.up;
			camera->frustum.front = rotationMatrix * camera->frustum.front;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			float3x3 rotationMatrix = float3x3::RotateAxisAngle(camera->frustum.WorldRight().Normalized(), DeltaY);
			camera->frustum.up = rotationMatrix * camera->frustum.up;
			camera->frustum.front = rotationMatrix * camera->frustum.front;

			if (camera->frustum.up.y < 0.0f)
			{
				camera->frustum.front = float3(0.0f, camera->frustum.front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				camera->frustum.up = Cross(camera->frustum.front, camera->frustum.WorldRight());
			}
		}
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}
// -----------------------------------------------------------------


void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	camera->frustum.front = (Position - Reference).Normalized();
	camera->frustum.WorldRight() = float3(0.0f, 1.0f, 0.0f).Cross(camera->frustum.front);
	camera->frustum.WorldRight().Normalize();
	camera->frustum.up = camera->frustum.front.Cross(camera->frustum.WorldRight());

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += camera->frustum.front * 0.05f;
	}

	CalculateViewMatrix();
}


// -----------------------------------------------------------------

void ModuleCamera3D::LookAt( const float3 &Spot)
{

	if (!Position.IsZero() && !Reference.IsZero())
	{
		Reference = Spot;
		camera->frustum.front = (Position - Reference).Normalized();
		camera->frustum.WorldRight() = float3(0.0f, 1.0f, 0.0f).Cross(camera->frustum.front);
		camera->frustum.WorldRight().Normalize();
		camera->frustum.up = camera->frustum.front.Cross(-camera->frustum.WorldRight());
		CalculateViewMatrix();
	}
	else
	{
		LOG("Error, No mesh founded");
	}

}



// -----------------------------------------------------------------

void ModuleCamera3D::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}


// -----------------------------------------------------------------

float* ModuleCamera3D::GetViewMatrix()
{
	return (float*)&ViewMatrix;
}

void ModuleCamera3D::CreateMeshBox(float3 minVertex, float3 maxVertex)
{
	meshBox->minPoint = minVertex;
	meshBox->maxPoint = maxVertex;
}

void ModuleCamera3D::LookAtMeshBox()
{
	if (!meshBox->Equals(*empty_meshBox))
	{
		Reference = meshBox->CenterPoint();
		Position = meshBox->CenterPoint() + meshBox->Size();
		LookAt(Reference);
	}
}

void ModuleCamera3D::ShowCameraInfo()
{
	ImGui::InputFloat("Camera Speed", &aux_speed, 1.0f);
	ImGui::InputFloat("Fast Camera Speed", &fast_speed, 1.0f);
	ImGui::InputFloat("Scroll Camera Speed", &scroll_speed, 1.0f);

}

// -----------------------------------------------------------------

void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(camera->frustum.WorldRight().x, camera->frustum.up.x, camera->frustum.front.x, 0.0f,
						  camera->frustum.WorldRight().y, camera->frustum.up.y, camera->frustum.front.y, 0.0f,
						  camera->frustum.WorldRight().z, camera->frustum.up.z, camera->frustum.front.z, 0.0f,
						  -Dot(camera->frustum.WorldRight(), Position), -Dot(camera->frustum.up, Position), -Dot(camera->frustum.front, Position), 1.0f);

	ViewMatrixInverse = ViewMatrix.Inverted();
}
