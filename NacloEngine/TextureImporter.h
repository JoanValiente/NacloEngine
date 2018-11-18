#ifndef __TEXTUREIMPORTER_H__
#define __TEXTUREIMPORTER_H__

#include "Importer.h"
#include <list>
#include <string>

struct Texture;

class TextureImporter : public Importer
{
public:

	TextureImporter();
	~TextureImporter();

	Texture* LoadTexture(const char* path);
	bool Import(const void* buffer, uint size, std::string& output_file);

	void SetTexture();

	uint const LoadCheckersTexture();
	uint CreateCheckersTexture(const void* checkImage);

private:
	std::list<Texture*>textures_loaded;
};

#endif // __TEXTUREIMPORTER_H_