#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "TextObject.h"

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

// Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;

#define TILE_SIZE 32

#define MAX_MAP_X 40
#define MAX_MAP_Y 20

typedef struct Map {
	int start_x_;
	int start_y_;

	int max_x_;
	int max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	std::string file_name_;
};

bool CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect);

// Floyd
#include <iostream>
#include <fstream>
#define VOCUNG 1000
using namespace std;
void NhapDoThi(int& n, int**& G, int name);
void Floyd(int n, int** G, int** D, int** S);

#endif // !COMMONFUNCTION_H_