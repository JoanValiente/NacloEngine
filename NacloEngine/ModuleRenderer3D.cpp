#include "OpenGL.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "PanelInspector.h"
#include "TextureImporter.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentRectTransform.h"
#include "ComponentCamera.h"
#include "ComponentImage.h"
#include "Quadtree.h"

#pragma comment (lib, "Glew/lib/glew32.lib")
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */



ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
	glDeleteFramebuffers(1, &frameBuffer);
}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit(); // … check for errors 

	if (err != GLEW_OK)
	{
		LOG("Glew library could not init %s\n", glewGetErrorString(err));
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	}

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glGenFramebuffers(1, &frameBuffer);
	// Projection matrix for

	return ret;
}

bool ModuleRenderer3D::Start()
{
	bool ret = true;

	CurrentCamera = App->camera->activeCamera;

	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);                                 

	checkers_path = App->texture->LoadCheckersTexture();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(CurrentCamera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(CurrentCamera->frustum.pos.x, CurrentCamera->frustum.pos.y, CurrentCamera->frustum.pos.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	
	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ComponentMesh* m = nullptr;
	ComponentImage* image = nullptr;

	for (std::vector<GameObject*>::const_iterator iterator = App->scene->gameObjects.begin(); iterator != App->scene->gameObjects.end(); ++iterator)
	{
		if ((*iterator)->active) {
			for (std::vector<Component*>::const_iterator it = (*iterator)->components.begin(); it != (*iterator)->components.end(); ++it)
			{
				if ((*it)->type == Component::COMPONENT_TYPE::COMPONENT_MESH)
				{
					m = (ComponentMesh*)(*it);
				}
			}
			if (m != nullptr)
			{
				DrawMesh((*iterator));
				m = nullptr;
			}
			
		}
	}


	for (std::vector<GameObject*>::const_iterator iterator = App->scene->gameObjects.begin(); iterator != App->scene->gameObjects.end(); ++iterator)
	{
		if ((*iterator)->active) {
			for (std::vector<Component*>::const_iterator it = (*iterator)->components.begin(); it != (*iterator)->components.end(); ++it)
			{
				if ((*it)->type == Component::COMPONENT_TYPE::COMPONENT_IMAGE)
				{
					image = (ComponentImage*)(*it);					
				}
				if (image != nullptr)
				{
					DrawUI((*iterator));
					image = nullptr;
				}
			}
		}
	}

	if (App->engineState == ENGINE_STATE::EDITOR) 
	{
		if (App->scene->show_quadtree)
		{
			App->scene->quadtree->DebugDraw();
		}
	}
	
	return ret;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	update_status update_return = UPDATE_CONTINUE;

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(App->window->window);

	if (App->imgui->close_engine)
		update_return = UPDATE_STOP;

	return update_return;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	textures.clear();

	ClearMeshes();

	return true;
}

void ModuleRenderer3D::DrawMesh(GameObject* go)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	float4x4 matrix = go->transform->globalMatrix;

	glMultMatrixf((GLfloat*)matrix.Transposed().ptr());
	if (go->mesh != nullptr)
	{
		glColor4f(go->mesh->mesh->color.r, go->mesh->mesh->color.g, go->mesh->mesh->color.b, go->mesh->mesh->color.a);
		
		if (go->material != nullptr)
		{
			if (go->material->texture != nullptr)
			{
				if (!ischecked)
					glBindTexture(GL_TEXTURE_2D, go->material->texture->texture_id);
				else
					glBindTexture(GL_TEXTURE_2D, checkers_path);
			}
		}

		if (wire_mode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, go->mesh->mesh->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, go->mesh->mesh->id_texture);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->mesh->mesh->id_indices);

		glDrawElements(GL_TRIANGLES, go->mesh->mesh->num_indices, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();
	}
}

void ModuleRenderer3D::DrawUI(GameObject* go)
{
	if (go->rectTransform != nullptr && go->image != nullptr)
	{

		glPushMatrix();

		float4x4 matrix = go->rectTransform->globalMatrix;

		glMultMatrixf((GLfloat*)matrix.Transposed().ptr());

		glColor3f(255, 255, 255);

		if (wire_mode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnableClientState(GL_VERTEX_ARRAY);

		int ID_texture = 0;
		if (go->image->tex != nullptr)
		{
			ID_texture = go->image->tex->texture_id;
		}

		glBindTexture(GL_TEXTURE_2D, ID_texture);

		glBindBuffer(GL_ARRAY_BUFFER, go->image->image_rect->vertexId);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, go->image->image_rect->textureId);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->image->image_rect->indexId);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();
	}
	else
	{
		LOG("ERROR RENDERING UI ELEMENT, NO RECT TRANSFORM COMPONENT");
	}
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	myWidth = width / 1.8;
	myHeight = height / 1.5;
	glViewport(width/4.5, height/3.55, myWidth, myHeight);
	CurrentCamera->SetAspectRatio(((float)width / 1.8) / ((float)height / 1.5));
	RecalculateProjection();
}

void ModuleRenderer3D::RecalculateProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(CurrentCamera->GetProjectionMatrix().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::AddMesh(Mesh * mesh)
{
	meshes.push_back(mesh);

	GetMeshMinMaxVertices(mesh);
}

void ModuleRenderer3D::AddTexture(Texture * tex)
{
	textures.push_back(tex);
}

void ModuleRenderer3D::GetMeshMinMaxVertices(Mesh * mesh)
{
	float3 minVertex;
	float3 maxVertex;
	if (meshes.size() == 1) {
		//minVertex = { mesh->vertices[0], mesh->vertices[0], mesh->vertices[0] };
		//maxVertex = { mesh->vertices[0], mesh->vertices[0], mesh->vertices[0] };
	}
	else {
		minVertex = App->camera->meshBox->minPoint;
		maxVertex = App->camera->meshBox->maxPoint;
	}
	/*
	for (int i = 0; i < mesh->num_vertices; i++) {

		if (mesh->vertices[i] < minVertex.x) {
			minVertex.x = mesh->vertices[i];
		}
		if (mesh->vertices[i] > maxVertex.x) {
			maxVertex.x = mesh->vertices[i];
		}
		i++;
		if (mesh->vertices[i] < minVertex.y) {
			minVertex.y = mesh->vertices[i];
		}
		if (mesh->vertices[i] > maxVertex.y) {
			maxVertex.y = mesh->vertices[i];
		}
		i++;
		if (mesh->vertices[i] < minVertex.z) {
			minVertex.z = mesh->vertices[i];
		}
		if (mesh->vertices[i] > maxVertex.z) {
			maxVertex.z = mesh->vertices[i];
		}
	}
	*/

	App->camera->CreateMeshBox(minVertex, maxVertex);
	//App->camera->LookAtMeshBox();
}

void ModuleRenderer3D::ClearMeshes()
{
	meshes.clear();
}

void ModuleRenderer3D::DeleteAllMeshes()
{
	if (!meshes.empty())
	{
		for (int i = 0; i < meshes.size(); ++i)
		{
			meshes[i]->path.clear();
			meshes[i]->filename.clear();

			if (meshes[i]->indices != nullptr)
			{
				delete[] meshes[i]->indices;
				meshes[i]->indices = nullptr;
			}

			if (meshes[i]->vertices != nullptr)
			{
				delete[] meshes[i]->vertices;
				meshes[i]->vertices = nullptr;
			}

			if (meshes[i]->texture != nullptr)
			{
				delete[] meshes[i]->texture;
				meshes[i]->texture = nullptr;
			}

			if (meshes[i]->colors != nullptr)
			{
				delete[] meshes[i]->colors;
				meshes[i]->colors = nullptr;
			}

			glDeleteBuffers(1, (GLuint*) &(meshes[i]->id_vertices));
			glDeleteBuffers(1, (GLuint*) &(meshes[i]->id_indices));
			glDeleteTextures(1, (GLuint*) &(textures[i]->texture_id));
			glDeleteBuffers(1, (GLuint*) &(meshes[i]->id_texture));
			glDeleteBuffers(1, (GLuint*) &(meshes[i]->id_color));

			delete meshes[i];
		}
	}
	ClearMeshes();
}

void ModuleRenderer3D::ChangeCamera()
{
	CurrentCamera = App->camera->activeCamera;
	RecalculateProjection();
}

#ifndef GAME_MODE
void ModuleRenderer3D::ShowRenderInfo()
{
	static bool depth_test = glIsEnabled(GL_DEPTH_TEST);
	static bool cull_face = glIsEnabled(GL_CULL_FACE);
	static bool lighting = glIsEnabled(GL_LIGHTING);
	static bool line_smooth = glIsEnabled(GL_LINE_SMOOTH);
	static bool polygon_smooth = glIsEnabled(GL_POLYGON_SMOOTH);
	static bool color_material = glIsEnabled(GL_COLOR_MATERIAL);
	static bool texture_2D = glIsEnabled(GL_TEXTURE_2D);

	if (ImGui::Checkbox("Depth Test", &depth_test))
	{
		if (depth_test)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	if (ImGui::Checkbox("Cull Face", &cull_face))
	{
		if (cull_face)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}


	if (ImGui::Checkbox("Lighting", &lighting))
	{
		if (lighting)
		{
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}

	if (ImGui::Checkbox("Smooth Lines", &line_smooth))
	{
		if (line_smooth)
		{
			glEnable(GL_LINE_SMOOTH);
		}
		else
		{
			glDisable(GL_LINE_SMOOTH);
		}
	}

	if (ImGui::Checkbox("Smooth Polygons", &polygon_smooth))
	{
		if (polygon_smooth)
		{
			glEnable(GL_POLYGON_SMOOTH);
		}
		else
		{
			glDisable(GL_POLYGON_SMOOTH);
		}
	}

	if (ImGui::Checkbox("Color Material", &color_material))
	{
		if (color_material)
		{
			glEnable(GL_MATRIX_MODE);
		}
		else
		{
			glDisable(GL_MATRIX_MODE);
		}
	}

	if (ImGui::Checkbox("Texture 2D", &texture_2D))
	{
		if (texture_2D)
		{
			glEnable(GL_TEXTURE_2D);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}

	ImGui::Checkbox("Show Quadtree", &App->scene->show_quadtree);
	ImGui::Checkbox("Checkers Mode", &ischecked);
	

	ImGui::Checkbox("Wire Mode", &wire_mode);

}
#endif

float4x4 ModuleRenderer3D::perspective(float fovy, float aspect, float n, float f)
{
	float4x4 Perspective;

	Perspective = float4x4::zero;

	float coty = 1.0f / tan(fovy * (float)pi / 360.0f);

	Perspective.v[0][0] = coty / aspect;
	Perspective.v[1][1] = coty;
	Perspective.v[2][2] = (n + f) / (n - f);
	Perspective.v[2][3] = -1.0f;
	Perspective.v[3][2] = 2.0f * n * f / (n - f);

	return Perspective;
}
