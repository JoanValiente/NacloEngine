#include "ModuleTextures.h"
#include "Application.h"

ModuleTextures::ModuleTextures(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Start()
{
	bool ret = true;
	texture_path = "No Texture Loaded";
	texture_name = "No Texture Loaded";
	return ret;
}

bool ModuleTextures::CleanUp()
{
	bool ret = true;
	return ret;
}

uint ModuleTextures::LoadTexture(const char* path) 
{
	std::string new_path = path;
	App->fs->NormalizePath(new_path);
	texture_path = new_path;
	std::string path_to_name = texture_path;
	texture_name = path_to_name.erase(0, path_to_name.find_last_of("\\") + 1);

		ILuint imageID;				// Create an image ID as a ULuint

		uint textureID;			// Create a texture ID as a GLuint

		ILboolean success;			// Create a flag to keep track of success/failure

		ILenum error;				// Create a flag to keep track of the IL error state

		ilGenImages(1, &imageID); 		// Generate the image ID

		ilBindImage(imageID); 			// Bind the image

		success = ilLoadImage(path); 	// Load the image file

		ILinfo ImageInfo; // If we managed to load the image, then we can start to do things with it...

		if (success)
		{

			// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
			
			iluGetImageInfo(&ImageInfo);
			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}

			// Convert the image into a suitable format to work with
			// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

			texture_height = ImageInfo.Height;
			texture_width = ImageInfo.Width;

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
		
		std::string output_file;
		char* buffer = nullptr;
		uint size = App->fs->Load(new_path.c_str(), &buffer);
		Import(buffer, size, output_file);

		ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

		std::cout << "Texture creation successful." << std::endl;

		last_texture_loaded = textureID; 

		return textureID; // Return the GLuint to the texture so you can use it!
		

}

bool ModuleTextures::Import(const void * buffer, uint size, string& output_file)
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
					ret = App->fs->SavePath(output_file, data, size, LIBRARY_TEXTURES_FOLDER, "texture", "dds");

				RELEASE_ARRAY(data);
			}
			ilDeleteImages(1, &ImageName);
		}
	}

	if (ret == false)
		LOG("Cannot load texture from buffer of size %u", size);

	return ret;
}


uint const ModuleTextures::LoadCheckersTexture()
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

uint ModuleTextures::CreateCheckersTexture(const void* checkImage)
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

void const ModuleTextures::ShowTextureInfo()
{
	ImGui::Text("SIZE");
	ImGui::Text("Width: %i", texture_width); ImGui::SameLine();
	ImGui::Text("Height: %i", texture_height);
	ImGui::Text("Texture path:");
	ImGui::Text("%s", texture_path.c_str());
	ImGui::Text("Texture Name:");
	ImGui::Text("%s", texture_name.c_str());
	ImGui::Text("Texture");
	ImGui::Image((ImTextureID)last_texture_loaded, { 256,256 });
}
