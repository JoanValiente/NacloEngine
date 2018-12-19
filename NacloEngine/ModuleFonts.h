#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "Module.h"
#include <vector>
#include <string>

struct FT_Face;

struct font_data {
	float h;                                        // Holds The Height Of The Font.
	uint * textures;                              // Holds The Texture Id's
	uint list_base;                               // Holds The First Display List Id

	// The Init Function Will Create A Font With
	// The Height h From The File fname.
	void init(const char * fname, unsigned int h);

	// Free All The Resources Associated With The Font.
	void clean() {
		glDeleteLists(list_base, 128);
		glDeleteTextures(128, textures);
		delete[] textures;
	}
};

class ModuleFonts : public Module
{
public:
	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	
	void print(const font_data &ft_font, float x, float y, const char *fmt, ...);
	void Make_dlist(FT_Face face, char ch, uint list_base, uint * tex_base);
	int Next_p2(int a);
	void pushScreenCoordinateMatrix();
	void pop_projection_matrix();

};

#endif