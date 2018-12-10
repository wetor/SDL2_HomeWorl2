#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <cmath>
#include <algorithm>
static SDL_Window *sdlWindow = NULL;
static SDL_Renderer *sdlRender = NULL;
static SDL_Texture *sdlTexture = NULL;
static TTF_Font *font = NULL;
static TTF_Font *font_mini = NULL;
/*static int w = 800;
static int h = 640;*/

static int w = 800;
static int h = 640;
static int point_r = 6;//ͼ�ζ���뾶

static int unit = 16;//���ص�λ����

static int axes_x = (int)(w / 2 / unit)*unit;//������ԭ������
static int axes_y = (int)(h / 2 / unit)*unit;


static int FPS = 1000 / 25;//20���滻Ϊ���Ƶ�֡��
static Uint32 _FPS_Timer;


class Func {
public:
	static void DrawCircle(SDL_Renderer *ren, int x, int y, int radius);
	static void DrawAxes(SDL_Renderer *ren,int w,int h);
};
 
