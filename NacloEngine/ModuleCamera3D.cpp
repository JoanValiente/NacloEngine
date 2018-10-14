#include "Globals.h"
#include "Application.h"
//#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
	CalculateViewMatrix();
	
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

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
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) newPos += Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) newPos += X * speed;


	//-----------------------------WASD movement-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
	}

	//-----------------------------Focus mesh-----------------------------
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
		LookAtMeshBox();
	}

	//-----------------------------Wheel mouse zoom-----------------------------
	if (App->input->GetMouseZ() == 1)
		newPos -= Z * scroll_speed;

	if (App->input->GetMouseZ() == -1)
		newPos += Z * scroll_speed;


	//-----------------------------Wheel mouse movement-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		if (App->input->GetMouseXMotion() > 0)
			newPos -= X * (App->input->GetMouseXMotion() * dt) * speed * 5;

		if (App->input->GetMouseXMotion() < 0)
			newPos -= X * (App->input->GetMouseXMotion() * dt) * speed * 5;

		if (App->input->GetMouseYMotion() > 0)
			newPos += Y * (App->input->GetMouseYMotion() * dt) * speed * 5;

		if (App->input->GetMouseYMotion() < 0)
			newPos += Y * (App->input->GetMouseYMotion() * dt) * speed * 5;

	}

	Position += newPos;
	Reference += newPos;

	//-----------------------------Mouse motion-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.01f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3x3 rotationMatrix = float3x3::RotateY(DeltaX);
			X = rotationMatrix * X;
			Y = rotationMatrix * Y;
			Z = rotationMatrix * Z;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			float3x3 rotationMatrix = float3x3::RotateAxisAngle(X, DeltaY);
			Y = rotationMatrix * Y;
			Z = rotationMatrix * Z;

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Cross(Z, X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	//-----------------------------LEFT Mouse motion-----------------------------
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.01f;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3x3 rotationMatrix = float3x3::RotateY(DeltaX);
			X = rotationMatrix * X;
			Y = rotationMatrix * Y;
			Z = rotationMatrix * Z;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			float3x3 rotationMatrix = float3x3::RotateAxisAngle(X, DeltaY);
			Y = rotationMatrix * Y;
			Z = rotationMatrix * Z;

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Cross(Z, X);
			}
		}

		// Recalculate matrix -------------
		CalculateViewMatrix();

	}
	return UPDATE_CONTINUE;
}
// -----------------------------------------------------------------


void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = float3(0.0f, 1.0f, 0.0f).Cross(Z);
	X.Normalize();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}


// -----------------------------------------------------------------

void ModuleCamera3D::LookAt( const float3 &Spot)
{
	if (!Position.IsZero() && !Reference.IsZero())
	{
		Reference = Spot;
		Z = (Position - Reference).Normalized();
		X = float3(0.0f, 1.0f, 0.0f).Cross(Z);
		X.Normalize();
		Y = Z.Cross(X);
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
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -Dot(X, Position), -Dot(Y, Position), -Dot(Z, Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();;
}
