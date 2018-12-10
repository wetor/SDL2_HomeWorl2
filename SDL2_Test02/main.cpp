#pragma comment(lib, "x86/SDL2.lib")
#pragma comment(lib, "x86/SDL2main.lib")
#pragma comment(lib, "x86/SDL2_ttf.lib")

#include "Polygon.h"
using namespace std;
/*
SDL初始框架
https://www.cnblogs.com/1024Planet/p/5750481.html
SDL_TTF应用
https://www.cnblogs.com/landmark/archive/2012/06/01/2526140.html
SDL帧率限制
https://www.cnblogs.com/Darksun/p/4302298.html
虚基类
https://blog.csdn.net/m0_37968915/article/details/75206276
不定参数
https://www.cnblogs.com/litmin/p/7508682.html
线段是否相离/相交
https://www.cnblogs.com/wuwangchuxin0924/p/6218494.html
凹凸多边形判断
https://zhidao.baidu.com/question/514364714.html
多边形面积计算
https://blog.csdn.net/tigercoder/article/details/70161646
其他
SDL Wiki
http://wiki.libsdl.org/FrontPage
百度搜索、百度翻译
*/

bool InitView(int width, int height)
{
	//初始化窗体
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	sdlWindow = SDL_CreateWindow(
		"The Second Course Design",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
		SDL_WINDOW_RESIZABLE);
	if (sdlWindow == NULL) return false;

	//加载渲染器
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
	//销毁窗口、渲染器、纹理
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
	//表面转纹理
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
	int current = -1;//当前焦点图形
	int index = -1;//当前鼠标所在点序号
	Line line;//直线1主
	Line line2;//直线2辅
	int current_line = 0;//当前直线（线段）
	int current_line2 = 0;//当前2直线（线段）


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
		//事件处理等
		if (SDL_GetTicks() - _FPS_Timer < FPS) {
			SDL_Delay(FPS - SDL_GetTicks() + _FPS_Timer);
		}
		_FPS_Timer = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN://按键按下
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
					if (event.key.keysym.mod & KMOD_CTRL) {//增加点
						img[current].AddPoint(save_x, save_y);
						cout << "CTRL+A" << endl;
					}
					if (event.key.keysym.mod & KMOD_SHIFT) {//增加图形
						img.push_back(Polygon(0));
						current = img.size() - 1;
						img[current].AddPoint(Point(save_x, save_y));
						cout << "SHIFT+A" << endl;
					}
					break;
				case SDLK_d:
					if (event.key.keysym.mod & KMOD_CTRL) {//删除点
						if (index != -1) {
							img[current].DelPoint(index);
							index = img[current].Length() - 1;
						}
							
						cout << "CTRL+D" << endl;

					}
					if (event.key.keysym.mod & KMOD_SHIFT) {//删除图形
						if (current >= 0 && current < img.size()) {
							img.erase(img.begin() + current);
							current = img.size() - 1;
						}
						cout << "SHIFT+D" << endl;

					}
					break;
				case SDLK_1:
					if (event.key.keysym.mod & KMOD_SHIFT) {//增加三角形
						img.push_back(Polygon(3,Point(save_x, save_y), Point(save_x-48, save_y+48), Point(save_x + 48, save_y + 48)));
						current = img.size() - 1;
						cout << "SHIFT+1" << endl;
					}
					break;
				case SDLK_2:
					if (event.key.keysym.mod & KMOD_SHIFT) {//增加四边形
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
			case SDL_MOUSEMOTION://鼠标移动
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
			case SDL_MOUSEBUTTONDOWN://鼠标按下
				if (event.button.button = SDL_BUTTON_LEFT)//左键
				{
					//cout << "b " << index << endl;
					if (index != -1) {
						img[current].SetPoint(Mouse_Press, index);
						cout<< "Position "<<img[current].GetPoint(index).ax<<" "<< img[current].GetPoint(index).ay<<endl;
					}
				}
				
				break;
			case SDL_MOUSEBUTTONUP://弹起
				if (event.button.button = SDL_BUTTON_LEFT)//左键
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
		

		SDL_SetRenderDrawColor(sdlRender, 0xFF, 0, 0, 0xFF);//辅助直线
		if (current_line2 == 1) {
			Func::DrawCircle(sdlRender, line2.point[0].x, line2.point[0].y, point_r / 2);
			SDL_RenderDrawLine(sdlRender, line2.point[0].x, line2.point[0].y, save_x, save_y);
		}
		else if (current_line2 == 2) {
			SDL_RenderDrawLine(sdlRender, line2.point[0].x, line2.point[0].y, line2.point[1].x, line2.point[1].y);
			Func::DrawCircle(sdlRender, line2.point[0].x, line2.point[0].y, point_r / 2);
			Func::DrawCircle(sdlRender, line2.point[1].x, line2.point[1].y, point_r / 2);
		}
		SDL_SetRenderDrawColor(sdlRender, 0, 0xFF, 0, 0xFF);//主要直线
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
			DrawText(L"坐标：", 10, draw_info_y);
			swprintf_s(wstr, L"x:%d  y:%d", (save_x - axes_x) / unit, (axes_y - save_y) / unit);
			DrawText(wstr, 80, draw_info_y);
			draw_info_y += 25;

			DrawText(L"形状：", 10, draw_info_y);
			DrawText(img[current].JudgeShapeStr(), 80, draw_info_y);
			draw_info_y += 25;

			DrawText(L"面积：", 10, draw_info_y);
			swprintf_s(wstr, L"%f", img[current].CalcArea(1));
			DrawText(wstr, 80, draw_info_y);
			draw_info_y += 25;

			DrawText(L"周长：", 10, draw_info_y);
			swprintf_s(wstr, L"%f", img[current].CalcPerimeter(1));
			DrawText(wstr, 80, draw_info_y);
			draw_info_y += 25;

			DrawText(L"图形序号：", 10, draw_info_y);
			swprintf_s(wstr, L"%d", current);
			DrawText(wstr, 120, draw_info_y);
			draw_info_y += 25;


			if (index != -1) {
				DrawText(L"点序号   ：", 10, draw_info_y);
				swprintf_s(wstr, L"%d", index);
				DrawText(wstr, 120, draw_info_y);
				draw_info_y += 25;

				DrawText(L"距离原点：", 10, draw_info_y);
				swprintf_s(wstr, L"%f", Calc::CalcDistance_PP(img[current].GetPoint(index), Point(axes_x, axes_y)) / unit);
				DrawText(wstr, 120, draw_info_y);
				draw_info_y += 25;
			}

			if (current_line == 2 && current_line2 != 2 && index != -1)//用主要直线判断
			{
				DrawText(L"距离直线：", 10, draw_info_y);
				swprintf_s(wstr, L"%f", Calc::CalcDistance_PL(img[current].GetPoint(index), Line(line.point[0], line.point[1])) / unit);
				DrawText(wstr, 120, draw_info_y);
				draw_info_y += 25;
			}
			if (current_line == 2 && current_line2 == 2 )//显示直线位置关系
			{
				DrawText(L"直线关系：", 10, draw_info_y);
				DrawText(line.PositionStr(line2), 120, draw_info_y);
				draw_info_y += 25;
			}
			if (current_line2 == 2)//辅助线段测距
			{
				DrawText(L"度量距离：", 10, draw_info_y);
				swprintf_s(wstr, L"%f", line2.Length()/16);
				DrawText(wstr, 120, draw_info_y);
				draw_info_y += 25;
			}
		}
		if (show_help) {
			DrawText(L"ESC退出  方向键移动图形  鼠标拖动点", 10, h - 180);
			DrawText(L"F1显示/隐藏信息      增加三角形：Shift+1", 10, h - 155);
			DrawText(L"F2显示/隐藏帮助      增加四边形：Shift+2", 10, h - 130);
			DrawText(L"F3显示/隐藏坐标轴", 10, h - 105);
			DrawText(L"增加直线点[主]：Ctrl   增加直线点[辅]：Alt  (辅直线仅用于两直线关系判断)", 10, h - 80);
			DrawText(L"增加图形：Shift+A   增加点：Ctrl+A", 10, h - 55);
			DrawText(L"删除图形：Shift+D   删除点：Ctrl+D", 10, h - 30);
		}
		SDL_RenderPresent(sdlRender);

	}

	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
	return 0;
}