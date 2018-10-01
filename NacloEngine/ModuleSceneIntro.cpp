#include "Application.h"
#include "ModuleSceneIntro.h"
#include "OpenGL.h" 
#include "Globals.h"

ModuleSceneIntro::ModuleSceneIntro(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

bool ModuleSceneIntro::Start() 
{
	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));

	grid = new plane(0, 1, 0, 0);
	grid->axis = true;
	grid->grid = true;

	return true;

}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	grid->Render();


	/*
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1.0, 1.0, 1.0); glVertex3f(0.0, 4.0, 0.0);
	glColor3f(1.0, 0.0, 0.0); glVertex3f(-2.0, 0.0, 2.0);
	glColor3f(0.0, 1.0, 0.0); glVertex3f(2.0, 0.0, 2.0);
	glColor3f(0.0, 0.0, 1.0); glVertex3f(0.0, 0.0, -2.8);
	glColor3f(1.0, 1.0, 1.0); glVertex3f(0.0, 4.0, 0.0);
	glColor3f(1.0, 0.0, 0.0); glVertex3f(-2.0, 0.0, 2.0);
	glEnd();
	*/
	/*
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1.0, 1.0, 1.0); glVertex3f(0.0, 4.0, 0.0);
	glColor3f(1.0, 0.0, 0.0); glVertex3f(-2.0, 0.0, 2.0);
	glColor3f(0.0, 1.0, 0.0); glVertex3f(2.0, 0.0, 2.0);
	glColor3f(0.0, 0.0, 1.0); glVertex3f(0.0, 0.0, -2.8);
	glColor3f(1.0, 1.0, 1.0); glVertex3f(0.0, 4.0, 0.0);
	glColor3f(1.0, 0.0, 0.0); glVertex3f(-2.0, 0.0, 2.0);
	glEnd();
	*/

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);

	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 2.0f, 0.0f);

	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(2.0f, 2.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, -2.0f);

	glVertex3f(2.0f, 2.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, -2.0f);
	glVertex3f(2.0f, 2.0f, -2.0f);

	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(2.0f, 2.0f, -2.0f);
	glVertex3f(2.0f, 0.0f, -2.0f);
	glVertex3f(0.0f, 0.0f, -2.0f);

	glVertex3f(0.0f, 2.0f, -2.0f);
	glVertex3f(2.0f, 2.0f, -2.0f);
	glVertex3f(0.0f, 0.0f, -2.0f);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 2.0f, -2.0f);
	glVertex3f(0.0f, 0.0f, -2.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 2.0f, -2.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.0f, 2.0f, -2.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(2.0f, 2.0f, 0.0f);

	glVertex3f(0.0f, 2.0f, -2.0f);
	glVertex3f(2.0f, 2.0f, 0.0f);
	glVertex3f(2.0f, 2.0f, -2.0f);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.0f, 0.0f, -2.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, -2.0f);
	glVertex3f(2.0f, 0.0f, -2.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glEnd();

	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::CleanUp()
{
	return false;
}
