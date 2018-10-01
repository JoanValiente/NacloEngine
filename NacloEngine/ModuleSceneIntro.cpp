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
	//glewInit();
	vertexs[0] = 1.0f;
	vertexs[1] = 1.0f;
	vertexs[2] = 0.0f;

	vertexs[3] = 0.0f;
	vertexs[4] = 1.0f;
	vertexs[5] = 0.0f;

	vertexs[6] = 1.0f;
	vertexs[7] = 1.0f;
	vertexs[8] = 1.0f;

	/*
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 180 * 3, vertexs, GL_STATIC_DRAW);
	*/

	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));

	grid = new plane(0, 1, 0, 0);
	grid->axis = true;
	grid->grid = true;

	return true;

}

update_status ModuleSceneIntro::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 9);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	*/

	return ret;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	grid->Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro Scene");
	
	return false;
}
