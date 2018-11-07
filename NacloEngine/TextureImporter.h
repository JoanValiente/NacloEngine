#ifndef __TEXTUREIMPORTER_H__
#define __TEXTUREIMPORTER_H__

#include "Importer.h"
#include <list>
#include <string>


class TextureImporter : public Importer
{
public:

	TextureImporter();
	~TextureImporter();

	bool Start();
	bool CleanUp();

	uint LoadTexture(const char* path);
	bool Import(const void* buffer, uint size, std::string& output_file);

	void SetTexture();

	uint const LoadCheckersTexture();
	uint CreateCheckersTexture(const void* checkImage);

	void const ShowTextureInfo();

private:
	std::string texture_path;
	std::string texture_name;
	uint last_texture_loaded = 0;
public:
	uint texture_height = 0;
	uint texture_width= 0;
};

#endif // __TEXTUREIMPORTER_H__