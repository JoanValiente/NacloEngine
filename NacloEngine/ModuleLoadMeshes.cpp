#include "ModuleLoadMeshes.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"


#include "Application.h"


#pragma comment (lib,"Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include <iostream>


ModuleLoadMeshes::ModuleLoadMeshes(Application*app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleLoadMeshes::~ModuleLoadMeshes()
{

}

bool ModuleLoadMeshes::Init()
{
	bool ret = true;

	ilInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool ModuleLoadMeshes::Start()
{
	aiLogStream stream = aiLogStream();
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleLoadMeshes::CleanUp()
{
	aiDetachAllLogStreams();

	if (mesh.indices != nullptr)
		delete[]mesh.indices;

	if (mesh.vertices != nullptr)
		delete[]mesh.vertices;


	return true;
}

void ModuleLoadMeshes::LoadFBX(const char * path)
{

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes()) {
		for (int num_meshes = 0; num_meshes < scene->mNumMeshes; ++num_meshes)
		{
			aiMesh* new_mesh = scene->mMeshes[num_meshes];
			mesh.num_vertices = new_mesh->mNumVertices;
			mesh.vertices = new float[mesh.num_vertices * 3];
			memcpy(mesh.vertices, new_mesh->mVertices, sizeof(float)*mesh.num_vertices * 3);
			LOG("New mesh with %d vertices", mesh.num_vertices);

			if (new_mesh->HasFaces())
			{
				mesh.num_indices = new_mesh->mNumFaces * 3;
				mesh.indices = new uint[mesh.num_indices];

				for (uint num_faces = 0; num_faces < new_mesh->mNumFaces; ++num_faces)
				{
					if (new_mesh->mFaces[num_faces].mNumIndices != 3)
					{
						LOG("Geometry face %i whit %i faces", num_faces, new_mesh->mFaces[num_faces].mNumIndices);
					}
					else {
						memcpy(&mesh.indices[num_faces * 3], new_mesh->mFaces[num_faces].mIndices, 3 * sizeof(uint));
					}
				}
			}

			aiMaterial* color_material = scene->mMaterials[new_mesh->mMaterialIndex];
			if (aiGetMaterialColor(color_material, AI_MATKEY_COLOR_AMBIENT, &mesh.color) == aiReturn_FAILURE || mesh.color == aiColor4D(0, 0, 0, 1))
			{
				mesh.color = { 255.0f,255.0f,255.0f,255.0f };
			}
			aiColor4D* colors_mesh = *new_mesh->mColors;

			if (colors_mesh != nullptr)
			{
				mesh.colors = new float[mesh.num_vertices * 3];
				for (int num_color = 0; num_color < mesh.num_vertices; ++num_color)
				{
					memcpy(mesh.colors, &colors_mesh[num_color], sizeof(float)*mesh.num_vertices * 3);
				}
			}

			aiMaterial* tex = scene->mMaterials[0];
			
			if (tex != nullptr)
			{
				aiString path;
				tex->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				if (path.length > 0)
				{
					std::string path_location = path.data;
					path_location.erase(0, path_location.find_last_of("\\") + 1);
					std::string folder = "Textures/";
					folder += path_location;

					ILuint id;
					ilGenImages(1, &id);
					ilBindImage(id);
					ilLoadImage(folder.c_str());

					mesh.id_texture = ilutGLBindTexImage();

					folder.clear();
					path_location.clear();
					path.Clear();
				}
			}
			
			if (new_mesh->HasTextureCoords(0))
			{
				mesh.num_texture = new_mesh->mNumVertices;
				mesh.texture = new float[mesh.num_texture * 2];
				LOG("New mesh with %d textures", mesh.num_texture);
				for (uint texCoord = 0; texCoord < new_mesh->mNumVertices; ++texCoord)
				{
					memcpy(&mesh.texture[texCoord * 2], &new_mesh->mTextureCoords[0][texCoord].x, sizeof(float));
					memcpy(&mesh.texture[(texCoord * 2) + 1], &new_mesh->mTextureCoords[0][texCoord].y, sizeof(float));
				}

			}
		}

		glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh.id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) &(mesh.id_texture));
		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_texture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh.num_texture, mesh.texture, GL_STATIC_DRAW);

		/*
		glGenBuffers(1, (GLuint*) &(mesh.id_color));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_color);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_color, mesh.colors, GL_STATIC_DRAW);
		*/
	}
}

GLuint ModuleLoadMeshes::LoadTexture(const char * path)
{
		ILuint imageID;				// Create an image ID as a ULuint

		GLuint textureID;			// Create a texture ID as a GLuint

		ILboolean success;			// Create a flag to keep track of success/failure

		ILenum error;				// Create a flag to keep track of the IL error state

		ilGenImages(1, &imageID); 		// Generate the image ID

		ilBindImage(imageID); 			// Bind the image

		success = ilLoadImage(path); 	// Load the image file

												// If we managed to load the image, then we can start to do things with it...
		if (success)
		{
			// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);
			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}

			// Convert the image into a suitable format to work with
			// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

			// Quit out if we failed the conversion
			if (!success)
			{
				error = ilGetError();
				std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
				exit(-1);
			}

			// Generate a new texture
			glGenTextures(1, &textureID);

			// Bind the texture to a name
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Set texture clamping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			// Set texture interpolation method to use linear interpolation (no MIPMAPS)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Specify the texture specification
			glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
				0,				// Pyramid level (for mip-mapping) - 0 is the top level
				ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
				ilGetInteger(IL_IMAGE_WIDTH),	// Image width
				ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
				0,				// Border width in pixels (can either be 1 or 0)
				ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
				GL_UNSIGNED_BYTE,		// Image data type
				ilGetData());			// The actual image data itself
		}
		else // If we failed to open the image file in the first place...
		{
			error = ilGetError();
			std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
			exit(-1);
		}

		ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

		std::cout << "Texture creation successful." << std::endl;

		return textureID; // Return the GLuint to the texture so you can use it!
}
