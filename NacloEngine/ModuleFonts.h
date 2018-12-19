#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

// FreeType Headers
/*
#include "FreeType/include/ft2build.h"
#include "FreeType/include/freetype/freetype.h"
#include "FreeType/include/freetype/ftglyph.h"
#include "FreeType/include/freetype/ftoutln.h"
#include "FreeType/include/freetype/fttrigon.h"
*/ 

#include "Module.h"
#include <vector>
#include <string>

struct font_data {
	float h;                                        // Holds The Height Of The Font.
	GLuint * textures;                              // Holds The Texture Id's
	GLuint list_base;                               // Holds The First Display List Id

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

	/*
	void print(const font_data &ft_font, float x, float y, const char *fmt, ...);
	void make_dlist(FT_Face face, char ch, GLuint list_base, GLuint * tex_base);
	inline int next_p2(int a);
	inline void pushScreenCoordinateMatrix();
	inline void pop_projection_matrix();
	*/

};

#endif