#include "TextureImporter.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "Devil/include/ilut.h"
#include "ImGui/imgui.h"

#include "Devil/include/ilut.h"
#include "Devil/include/il.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


TextureImporter::TextureImporter() : Importer()
{
}

TextureImporter::~TextureImporter()
{
}

Texture* TextureImporter::LoadTexture(const char* path)
{
	Texture* ret = new Texture();

	std::string name = path;
	ret->path = name;
	std::string extension = name;
	extension.erase(0, extension.find_last_of("."));
	ret->texture_name = name.erase(0, name.find_last_of("\\") + 1);
	std::string new_path = path;
	char* buffer = nullptr;
	uint size = App->fs->Load(path, &buffer);
	bool load_texture = true;

	list<Texture*>::const_iterator item = textures_loaded.begin();

	while (item != textures_loaded.end())
	{
		if ((*item)->path == ret->path)
		{
			load_texture = false;
			ret->texture_dds = (*item)->texture_dds;
		}
		item++;
	}


	if (buffer != nullptr && size > 0)
	{
		ILuint imageID;
		ilGenImages(1, &imageID);
		ilBindImage(imageID);

		uint textureID;

		if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
		{
			ILinfo ImageInfo;

			iluGetImageInfo(&ImageInfo);

			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}

			// Convert the image into a suitable format to work with
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

			ret->width = ImageInfo.Width;
			ret->height = ImageInfo.Height;

			// Generate a new texture
			glBindFramebuffer(GL_FRAMEBUFFER, App->renderer3D->frameBuffer);
			glGenTextures(1, &textureID);

			// Bind the texture to a name
			glBindTexture(GL_TEXTURE_2D, textureID);

			SetTexture();

			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0
			);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
			LOG("Texture creation successful.");

			if (extension != ".dds")
			{
				if (load_texture)
				{
					Import(buffer, size, ret->texture_dds); //Import texture in our own format
				}
			}

			ret->texture_id = textureID;

			if (load_texture)
			{
				textures_loaded.push_back(ret);
			}

			RELEASE_ARRAY(buffer);
		}
		else // If we failed to open the image file in the first place...
		{
			LOG("ERROR Trying to load a buffer of size %i", size);
			ret = nullptr;
		}
	}
	else
	{
		LOG("ERROR LOADING TEXTURES");
		ret = nullptr;
	}

	return ret;
}

bool TextureImporter::Import(const void * buffer, uint size, string& output_file)
{
	bool ret = false;

	if (buffer)
	{
		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
		{
			ilEnable(IL_FILE_OVERWRITE);

			ILuint   size;
			ILubyte *data;
			// To pick a specific DXT compression use 
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
			size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
			if (size > 0)
			{
				data = new ILubyte[size]; // allocate data buffer
				if (ilSaveL(IL_DDS, data, size) > 0) // Save with the ilSaveIL function
					ret = App->fs->Save(output_file, data, size, LIBRARY_TEXTURES_FOLDER, "texture", "dds");

				RELEASE_ARRAY(data);
			}
			ilDeleteImages(1, &ImageName);
		}
	}

	if (ret == false)
		LOG("Cannot load texture from buffer of size %u", size);

	return ret;
}

void TextureImporter::SetTexture()
{
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


uint const TextureImporter::LoadCheckersTexture()
{

	GLubyte checkImage[32][32][4];
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	uint textureID = CreateCheckersTexture(checkImage);

	return textureID;
}

uint TextureImporter::CreateCheckersTexture(const void* checkImage)
{
	uint ImageName = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, (GLuint*) &(ImageName));
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	return ImageName;
}
