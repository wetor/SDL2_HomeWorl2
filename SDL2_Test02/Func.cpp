#include "Func.h"

void Func::DrawCircle(SDL_Renderer *ren, int x, int y, int radius) {
	int  tx = 0, ty = radius, d = 3 - (radius << 1);
	while (tx < ty) {
		for (int i = x - ty; i <= x + ty; ++i) {
			SDL_RenderDrawPoint(ren, i, y - tx);
			if (tx)
				SDL_RenderDrawPoint(ren, i, y + tx);
		}
		if (d < 0)
			d += (tx << 2) + 6;
		else {
			for (int i = x - tx; i <= x + tx; ++i) {
				SDL_RenderDrawPoint(ren, i, y - ty);
				SDL_RenderDrawPoint(ren, i, y + ty);
			}
			d += ((tx - ty) << 2) + 10, ty--;
		}
		tx++;
	}
	if (tx == ty)
		for (int i = x - ty; i <= x + ty; ++i) {
			SDL_RenderDrawPoint(ren, i, y - tx);
			SDL_RenderDrawPoint(ren, i, y + tx);
		}
}

void Func::DrawAxes(SDL_Renderer *ren,int w,int h) {

	SDL_SetRenderDrawColor(ren, 192, 192, 192, 0xFF);//绿色 线
	for (int i = 0; i < w / unit+1; i++) //画线
		SDL_RenderDrawLine(ren, i*unit, 0, i*unit, h);
	for (int i = 0; i < h / unit+1; i++) //画线
		SDL_RenderDrawLine(ren, 0, i*unit ,w, i*unit);

	SDL_SetRenderDrawColor(ren, 223, 85, 0, 0xFF);
	SDL_RenderDrawLine(ren, axes_x-1, 0, axes_x-1, h);
	SDL_RenderDrawLine(ren, axes_x, 0, axes_x, h);
	SDL_RenderDrawLine(ren, axes_x+1, 0, axes_x+1, h);

	SDL_RenderDrawLine(ren, 0, axes_y-1, w, axes_y-1);
	SDL_RenderDrawLine(ren, 0, axes_y, w, axes_y);
	SDL_RenderDrawLine(ren, 0, axes_y+1, w, axes_y+1);


}


