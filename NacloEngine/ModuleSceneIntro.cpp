#include "OpenGL.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Globals.h"
#include "PanelOpenGL.h"

ModuleSceneIntro::ModuleSceneIntro(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

bool ModuleSceneIntro::Start() 
{
	//glewInit();

	/*
	vertexs[0] = 0.0f;  vertexs[1] = 0.0f;  vertexs[2] = 0.0f;
	vertexs[3] = 0.0f;	vertexs[4] = 1.0f;	vertexs[5] = 0.0f;
	vertexs[6] = 1.0f;	vertexs[7] = 1.0f;	vertexs[8] = 0.0f;

	vertexs[9] = 0.0f;  vertexs[10] = 0.0f; vertexs[11] = 0.0f;
	vertexs[12] = 1.0f;	vertexs[13] = 1.0f;	vertexs[14] = 0.0f;
	vertexs[15] = 1.0f;	vertexs[16] = 0.0f;	vertexs[17] = 0.0f;

	vertexs[18] = 0.0f; vertexs[19] = 0.0f; vertexs[20] = 0.0f;
	vertexs[21] = 0.0f;	vertexs[22] = 0.0f;	vertexs[23] = 1.0f;
	vertexs[24] = 0.0f;	vertexs[25] = 1.0f;	vertexs[26] = 0.0f;

	vertexs[27] = 0.0f; vertexs[28] = 1.0f; vertexs[29] = 0.0f;
	vertexs[30] = 0.0f;	vertexs[31] = 0.0f;	vertexs[32] = 1.0f;
	vertexs[33] = 0.0f;	vertexs[34] = 1.0f;	vertexs[35] = 1.0f;

	vertexs[36] = 0.0f; vertexs[37] = 0.0f; vertexs[38] = 1.0f;
	vertexs[39] = 1.0f;	vertexs[40] = 1.0f;	vertexs[41] = 1.0f;
	vertexs[42] = 0.0f;	vertexs[43] = 1.0f;	vertexs[44] = 1.0f;

	vertexs[45] = 1.0f; vertexs[46] = 1.0f; vertexs[47] = 1.0f;
	vertexs[48] = 0.0f;	vertexs[49] = 0.0f;	vertexs[50] = 1.0f;
	vertexs[51] = 1.0f;	vertexs[52] = 0.0f;	vertexs[53] = 1.0f;

	vertexs[54] = 1.0f; vertexs[55] = 0.0f; vertexs[56] = 0.0f;
	vertexs[57] = 1.0f;	vertexs[58] = 1.0f;	vertexs[59] = 0.0f;
	vertexs[60] = 1.0f;	vertexs[61] = 0.0f;	vertexs[62] = 1.0f;

	vertexs[63] = 1.0f; vertexs[64] = 1.0f; vertexs[65] = 0.0f;
	vertexs[66] = 1.0f;	vertexs[67] = 1.0f;	vertexs[68] = 1.0f;
	vertexs[69] = 1.0f;	vertexs[70] = 0.0f;	vertexs[71] = 1.0f;

	vertexs[72] = 0.0f; vertexs[73] = 0.0f; vertexs[74] = 0.0f;
	vertexs[75] = 1.0f;	vertexs[76] = 0.0f;	vertexs[77] = 0.0f;
	vertexs[78] = 0.0f;	vertexs[79] = 0.0f;	vertexs[80] = 1.0f;

	vertexs[81] = 1.0f; vertexs[82] = 0.0f; vertexs[83] = 1.0f;
	vertexs[84] = 0.0f;	vertexs[85] = 0.0f;	vertexs[86] = 1.0f;
	vertexs[87] = 1.0f;	vertexs[88] = 0.0f;	vertexs[89] = 0.0f;

	vertexs[90] = 1.0f; vertexs[91] = 1.0f; vertexs[92] = 1.0f;
	vertexs[93] = 1.0f;	vertexs[94] = 1.0f;	vertexs[95] = 0.0f;
	vertexs[96] = 0.0f;	vertexs[97] = 1.0f;	vertexs[98] = 1.0f;

	vertexs[99] = 0.0f; vertexs[100] = 1.0f; vertexs[101] = 0.0f;
	vertexs[102] = 0.0f;vertexs[103] = 1.0f; vertexs[104] = 1.0f;
	vertexs[105] = 1.0f;vertexs[106] = 1.0f; vertexs[107] = 0.0f;

	*/

	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertexs, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));

	grid = new plane(float3(0.0f, 0.0f, 0.0f), float3(100.0f, 0, 100.0f));
	grid->axis = true;
	grid->grid = true;

	cube = new Cube(float3(10.0f, 0.0f, 0.0f), float3(2.0f,2.0f,2.0f));
	cube->axis = true;

	Ray = new ray(float3(0.0f, 2.0f, 0.0f), float3(5.0f, 2.0f, 0.0f));


	arrow = new Arrow(float3(0.0f, 3.0f, 0.0f), float3(5.0f, 3.0f, 0.0f));

	test = new sphere(float3(4.0f,1.0f,0.0f), 1.0, 32U, 32U);


	return true;

}

update_status ModuleSceneIntro::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;


	return ret;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	grid->Render();
	cube->Render();
	Ray->Render();

	arrow->Render();

	test->Render();


	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return ret;
}

void ModuleSceneIntro::WireMode()
{
	if (App->imgui->wire_mode) {
		cube->wire = true;
		test->wire = true;
	}
	else {
		cube->wire = false;
		test->wire = false;
	}
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro Scene");
	
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return false;
}
