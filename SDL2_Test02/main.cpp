#pragma comment(lib, "x86/SDL2.lib")
#pragma comment(lib, "x86/SDL2main.lib")
#pragma comment(lib, "x86/SDL2_ttf.lib")

#include "Polygon.h"
using namespace std;
/*
SDL��ʼ���
https://www.cnblogs.com/1024Planet/p/5750481.html
SDL_TTFӦ��
https://www.cnblogs.com/landmark/archive/2012/06/01/2526140.html
SDL֡������
https://www.cnblogs.com/Darksun/p/4302298.html
�����
https://blog.csdn.net/m0_37968915/article/details/75206276
��������
https://www.cnblogs.com/litmin/p/7508682.html
�߶��Ƿ�����/�ཻ
https://www.cnblogs.com/wuwangchuxin0924/p/6218494.html
��͹������ж�
https://zhidao.baidu.com/question/514364714.html
������������
https://blog.csdn.net/tigercoder/article/details/70161646
����
SDL Wiki
http://wiki.libsdl.org/FrontPage
�ٶ��������ٶȷ���
*/

bool InitView(int width, int height)
{
	//��ʼ������
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	sdlWindow = SDL_CreateWindow(
		"The Second Course Design",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
		SDL_WINDOW_RESIZABLE);
	if (sdlWindow == NULL) return false;

	//������Ⱦ��
	sdlRender = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (sdlRender == NULL) return false;

	font = TTF_OpenFont("msyh.ttf", 22);
	font_mini = TTF_OpenFont("msyh.ttf", 12);
	if (font == NULL) SDL_Log("ERROR:font open failure %s\n", SDL_GetError());



	return true;
}

void Quit(int code)
{
	const char *errMsg = SDL_GetError();
	if (errMsg && strlen(errMsg)) {
		SDL_Log("Error : %s\n", errMsg);
	}
	TTF_CloseFont(font);
	TTF_CloseFont(font_mini);
	//���ٴ��ڡ���Ⱦ��������
	if (sdlWindow) SDL_DestroyWindow(sdlWindow);
	if (sdlRender) SDL_DestroyRenderer(sdlRender);

	SDL_Quit();
	exit(code);
}
void DrawText(wstring str,int x,int y,int mode=0) {

	if (str.length() == 0) return;
	SDL_Color color = { 0, 0, 0,255 };
	SDL_Surface *surf = NULL;
	if(mode)
		surf = TTF_RenderUNICODE_Blended(font_mini, (Uint16*)str.c_str(), color);
	else
		surf = TTF_RenderUNICODE_Blended(font, (Uint16*)str.c_str(), color);
	//����ת����
	//SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);
	SDL_SetSurfaceAlphaMod(surf, 150);
	SDL_Texture *text = SDL_CreateTextureFromSurface(sdlRender, surf);
	SDL_Rect dst = { x,y,surf->w,surf->h };
	SDL_RenderCopy(sdlRender, text,NULL, &dst);
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(text);

}


int main(int argc, char* argv[])
{
	vector<Polygon> img;
	int current = -1;//��ǰ����ͼ��
	int index = -1;//��ǰ������ڵ����
	Line line;//ֱ��1��
	Line line2;//ֱ��2��
	int current_line = 0;//��ǰֱ�ߣ��߶Σ�
	int current_line2 = 0;//��ǰ2ֱ�ߣ��߶Σ�


	img.push_back(Polygon(4,Point(100, 100), Point(100, 400),  Point(300, 400),Point(300, 250)));
	current = img.size() - 1;


	if (InitView(w, h) == false) {
		Quit(0);
		return -1;
	}


	SDL_SetWindowSize(sdlWindow, w, h);
	SDL_Log("w=%d, h=%d\n", w, h);
	SDL_Event event;
	int save_x=0, save_y=0;
	int show_info = 1;
	int show_axes = 1;
	int show_help = 1;
	int draw_info_y = 0;
	wchar_t wstr[256];
	
	while (1) {
		//�¼������
		if (SDL_GetTicks() - _FPS_Timer < FPS) {
			SDL_Delay(FPS - SDL_GetTicks() + _FPS_Timer);
		}
		_FPS_Timer = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN://��������
			{
				int key = event.key.keysym.sym;
				switch (key)
				{
				case SDLK_F1:
					show_info = show_info ? 0 : 1;
					break;
				case SDLK_F2:
					show_help = show_help ? 0 : 1;
					break;
				case SDLK_F3:
					show_axes = show_axes ? 0 : 1;
					break;
				case SDLK_ESCAPE:
					Quit(0);
					break;
				case SDLK_SPACE:
					cout << "Shape "<<img[current].JudgeShape() << endl;
					cout << "Area " << img[current].CalcArea(1) << "  Perimeter " << img[current].CalcPerimeter(1) << endl;
				case SDLK_UP:
				case SDLK_DOWN:
				case SDLK_LEFT:
				case SDLK_RIGHT: {
					int x = 0, y = 0;
					x = (key == SDLK_LEFT ? x - unit : (key == SDLK_RIGHT ? x + unit : x));
					y = (key == SDLK_UP ? y - unit : (key == SDLK_DOWN ? y + unit : y));
					img[current].Move(x, y);
					break;
				}
				case SDLK_a:
					if (event.key.keysym.mod & KMOD_CTRL) {//���ӵ�
						img[current].AddPoint(save_x, save_y);
						cout << "CTRL+A" << endl;
					}
					if (event.key.keysym.mod & KMOD_SHIFT) {//����ͼ��
						img.push_back(Polygon(0));
						current = img.size() - 1;
						img[current].AddPoint(Point(save_x, save_y));
						cout << "SHIFT+A" << endl;
					}
					break;
				case SDLK_d:
					if (event.key.keysym.mod & KMOD_CTRL) {//ɾ����
						if (index != -1) {
							img[current].DelPoint(index);
							index = img[current].Length() - 1;
						}
							
						cout << "CTRL+D" << endl;

					}
					if (event.key.keysym.mod & KMOD_SHIFT) {//ɾ��ͼ��
						if (current >= 0 && current < img.size()) {
							img.erase(img.begin() + current);
							current = img.size() - 1;
						}
						cout << "SHIFT+D" << endl;

					}
					break;
				case SDLK_1:
					if (event.key.keysym.mod & KMOD_SHIFT) {//����������
						img.push_back(Polygon(3,Point(save_x, save_y), Point(save_x-48, save_y+48), Point(save_x + 48, save_y + 48)));
						current = img.size() - 1;
						cout << "SHIFT+1" << endl;
					}
					break;
				case SDLK_2:
					if (event.key.keysym.mod & KMOD_SHIFT) {//�����ı���
						img.push_back(Polygon(4,Point(save_x, save_y), Point(save_x, save_y + 48), Point(save_x + 48, save_y + 48), Point(save_x + 48, save_y )));
						current = img.size() - 1;
						cout << "SHIFT+2" << endl;
					}
					break;
				case SDLK_LCTRL:
				case SDLK_RCTRL:
					if (current_line >= 2)
						current_line = 0;
					else {
						line.point[current_line] = Point(save_x, save_y);
						current_line++;
					}
					break;
				case SDLK_LALT:
				case SDLK_RALT:
					if (current_line2 >= 2)
						current_line2 = 0;
					else {
						line2.point[current_line2] = Point(save_x, save_y);
						current_line2++;
					}
					break;
				default:
					break;
				}
				//HandleKeyEvent(&event.key.keysym);
				break;
			}
			case SDL_MOUSEMOTION://����ƶ�
			{
				save_x = (int)((event.motion.x + unit / 2) / unit)*unit;
				save_y = (int)((event.motion.y + unit / 2) / unit)*unit;
				int temp = -1;
				index = -1;
				for (int i = 0; i < img.size(); i++) {
					temp=img[i].Update(save_x, save_y);
					if (temp != -1) {
						current = i;
						index = temp;
					}
				}
				//cout << "a "<<index << endl;
				break; 
			}
			case SDL_MOUSEBUTTONDOWN://��갴��
				if (event.button.button = SDL_BUTTON_LEFT)//���
				{
					//cout << "b " << index << endl;
					if (index != -1) {
						img[current].SetPoint(Mouse_Press, index);
						cout<< "Position "<<img[current].GetPoint(index).ax<<" "<< img[current].GetPoint(index).ay<<endl;
					}
				}
				
				break;
			case SDL_MOUSEBUTTONUP://����
				if (event.button.button = SDL_BUTTON_LEFT)//���
				{
					if (index != -1) {
						img[current].SetPoint(Mouse_Focus, index);
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					SDL_GetWindowSize(sdlWindow, &w, &h);
				}
				break;
			case SDL_QUIT:
				Quit(0);
				break;
			}
		}
		
		SDL_SetRenderDrawColor(sdlRender, 255, 153, 153, 0xFF);
		SDL_RenderClear(sdlRender);
		
		if (show_axes)
			Func::DrawAxes(sdlRender,w,h);
		SDL_SetRenderDrawColor(sdlRender, 0, 0xFF, 0, 0xFF);
		Func::DrawCircle(sdlRender, save_x, save_y, point_r / 2);


		for(int i=0;i< img.size();i++)
			img[i].DrawImage(sdlRender);
		

		SDL_SetRenderDrawColor(sdlRender, 0xFF, 0, 0, 0xFF);//����ֱ��
		if (current_line2 == 1) {
			Func::DrawCircle(sdlRender, line2.point[0].x, line2.point[0].y, point_r / 2);
			SDL_RenderDrawLine(sdlRender, line2.point[0].x, line2.point[0].y, save_x, save_y);
		}
		else if (current_line2 == 2) {
			SDL_RenderDrawLine(sdlRender, line2.point[0].x, line2.point[0].y, line2.point[1].x, line2.point[1].y);
			Func::DrawCircle(sdlRender, line2.point[0].x, line2.point[0].y, point_r / 2);
			Func::DrawCircle(sdlRender, line2.point[1].x, line2.point[1].y, point_r / 2);
		}
		SDL_SetRenderDrawColor(sdlRender, 0, 0xFF, 0, 0xFF);//��Ҫֱ��
		if (current_line == 1) {
			Func::DrawCircle(sdlRender, line.point[0].x, line.point[0].y, point_r / 2);
			SDL_RenderDrawLine(sdlRender, line.point[0].x, line.point[0].y, save_x, save_y);
		}
		else if (current_line == 2) {
			SDL_RenderDrawLine(sdlRender, line.point[0].x, line.point[0].y, line.point[1].x, line.point[1].y);
			Func::DrawCircle(sdlRender, line.point[0].x, line.point[0].y, point_r / 2);
			Func::DrawCircle(sdlRender, line.point[1].x, line.point[1].y, point_r / 2);
		}
		

		if (show_axes) {
			for (int i = axes_x - unit * 5; i > 0; i -= unit * 5) { //-x
				swprintf_s(wstr, L"%d", (i - axes_x) / unit);
				DrawText(wstr, i - 8, axes_y, 1);
			}
			for (int i = axes_x + unit * 5; i < w; i += unit * 5) {//+x
				swprintf_s(wstr, L"%d", (i - axes_x) / unit);
				DrawText(wstr, i - 3, axes_y, 1);
			}
			for (int i = axes_y - unit * 5; i > 0; i -= unit * 5) {//+y
				swprintf_s(wstr, L"%d", (axes_y - i) / unit);
				DrawText(wstr, axes_x + 3, i - 8, 1);
			}
			for (int i = axes_y + unit * 5; i < h; i += unit * 5) {//-y
				swprintf_s(wstr, L"%d", (axes_y - i) / unit);
				DrawText(wstr, axes_x + 3, i - 8, 1);
			}
		}

		draw_info_y = 0;
		if (show_info) {
			DrawText(L"���꣺", 10, draw_info_y);
			swprintf_s(wstr, L"x:%d  y:%d", (save_x - axes_x) / unit, (axes_y - save_y) / unit);
			DrawText(wstr, 80, draw_info_y);
			draw_info_y += 25;

			DrawText(L"��״��", 10, draw_info_y);
			DrawText(img[current].JudgeShapeStr(), 80, draw_info_y);
			draw_info_y += 25;

			DrawText(L"�����", 10, draw_info_y);
			swprintf_s(wstr, L"%f", img[current].CalcArea(1));
			DrawText(wstr, 80, draw_info_y);
			draw_info_y += 25;

			DrawText(L"�ܳ���", 10, draw_info_y);
			swprintf_s(wstr, L"%f", img[current].CalcPerimeter(1));
			DrawText(wstr, 80, draw_info_y);
			draw_info_y += 25;

			DrawText(L"ͼ����ţ�", 10, draw_info_y);
			swprintf_s(wstr, L"%d", current);
			DrawText(wstr, 120, draw_info_y);
			draw_info_y += 25;


			if (index != -1) {
				DrawText(L"�����   ��", 10, draw_info_y);
				swprintf_s(wstr, L"%d", index);
				DrawText(wstr, 120, draw_info_y);
				draw_info_y += 25;

				DrawText(L"����ԭ�㣺", 10, draw_info_y);
				swprintf_s(wstr, L"%f", Calc::CalcDistance_PP(img[current].GetPoint(index), Point(axes_x, axes_y)) / unit);
				DrawText(wstr, 120, draw_info_y);
				draw_info_y += 25;
			}

			if (current_line == 2 && current_line2 != 2 && index != -1)//����Ҫֱ���ж�
			{
				DrawText(L"����ֱ�ߣ�", 10, draw_info_y);
				swprintf_s(wstr, L"%f", Calc::CalcDistance_PL(img[current].GetPoint(index), Line(line.point[0], line.point[1])) / unit);
				DrawText(wstr, 120, draw_info_y);
				draw_info_y += 25;
			}
			if (current_line == 2 && current_line2 == 2 )//��ʾֱ��λ�ù�ϵ
			{
				DrawText(L"ֱ�߹�ϵ��", 10, draw_info_y);
				DrawText(line.PositionStr(line2), 120, draw_info_y);
				draw_info_y += 25;
			}
			if (current_line2 == 2)//�����߶β��
			{
				DrawText(L"�������룺", 10, draw_info_y);
				swprintf_s(wstr, L"%f", line2.Length()/16);
				DrawText(wstr, 120, draw_info_y);
				draw_info_y += 25;
			}
		}
		if (show_help) {
			DrawText(L"ESC�˳�  ������ƶ�ͼ��  ����϶���", 10, h - 180);
			DrawText(L"F1��ʾ/������Ϣ      ���������Σ�Shift+1", 10, h - 155);
			DrawText(L"F2��ʾ/���ذ���      �����ı��Σ�Shift+2", 10, h - 130);
			DrawText(L"F3��ʾ/����������", 10, h - 105);
			DrawText(L"����ֱ�ߵ�[��]��Ctrl   ����ֱ�ߵ�[��]��Alt  (��ֱ�߽�������ֱ�߹�ϵ�ж�)", 10, h - 80);
			DrawText(L"����ͼ�Σ�Shift+A   ���ӵ㣺Ctrl+A", 10, h - 55);
			DrawText(L"ɾ��ͼ�Σ�Shift+D   ɾ���㣺Ctrl+D", 10, h - 30);
		}
		SDL_RenderPresent(sdlRender);

	}

	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
	return 0;
}