#ifndef __GLOBALS_H_
#define __GLOBALS_H_

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

typedef unsigned int uint; 
typedef unsigned long long UID;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};
// GAME_MODE ---------------
#ifndef _GAME_MODE_
// GAME_MODE
#endif GAME_MODE

// Configuration -----------
#define SCREEN_WIDTH 1680
#define SCREEN_HEIGHT 1050
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE false
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define LIBRARY_TEXTURES_FOLDER "/Library/Textures/"
#define LIBRARY_MESH_FOLDER "/Library/Meshes/"
#define LIBRARY_SCENE_FOLDER "/Library/Scenes/"
#define ASSETS_MESH_FOLDER "/Assets/Models/"
#define ASSETS_TEXTURES_FOLDER "/Assets/Textures/"
#define ASSETS "/Assets"
#define ASSETS_SCENES_FOLDER "Assets/Scenes/"
#define SETTINGS_FOLDER "Settings/"


#endif