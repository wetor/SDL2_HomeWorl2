#pragma once

#include <vector>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "Func.h"


using namespace std;

enum State {
	Mouse_None,//一般
	Mouse_Focus,//焦点
	Mouse_Press//按下
};

class Point {
public:
	Point(float x = 0, float y = 0) {
		this->state = Mouse_None; 
		this->x = (float)((int)((x + unit / 2) / unit) * unit);
		this->y = (float)((int)((y + unit / 2) / unit) * unit);
		this->ax = (int)(this->x - axes_x) / unit;
		this->ay = (int)(axes_y- this->y) / unit;
		this->rect = { (int)this->x - point_r,(int)this->y - point_r,  point_r*2,point_r*2 };
	};
	float x, y;//屏幕坐标，像素
	int ax, ay;//坐标轴，单位
	SDL_Rect rect;
	int state;
	bool operator==(const Point& b) {
		return (b.x == this->x&&b.y == this->y);
	}
};

enum Position {
	Parallel,//平行
	Coincidence, //完全重合
	PartCoincidence, //部分重合
	Vertical,//垂直
	Intersect,//相交
	Disjoint//相离
};

class Line;

class Calc {
public:
	static float CalcDistance_PP(Point p1, Point p2);//use data ,calc point and point distance
	static float CalcDistance_PL(Point p, Line l);//use data ,calc point and line distance
	static int JudgePosition_LL(Line l1, Line l2);
	static bool JudgePosition_LL_Segment(Point aa, Point bb, Point cc, Point dd);
};

class Line
{
public:
	Point point[2];
	Line() {};
	Line(Point p1, Point p2) { point[0] = p1; point[1] = p2; };
	Line(float x1, float y1, float x2, float y2) { point[0] = Point(x1,y1); point[1] = Point(x2, y2); };
	float Slope() { 
		return (point[1].x - point[0].x) == 0 ? 999 : ((point[1].y - point[0].y) / (point[1].x - point[0].x));
	};//斜率
	float Intercept() { 
		if (Slope() == 999)
			return 0;
		else 
			return point[0].y - Slope() * point[0].x;
	};//截距

	float Length() { return  Calc::CalcDistance_PP(point[0], point[1]); };//长度
	float DistanceToPoint(Point p) { return Calc::CalcDistance_PL(p, *this); };
	float DistanceToPoint(float x, float y) { return Calc::CalcDistance_PL(Point(x, y), *this); };
	int Position(Line l) { return Calc::JudgePosition_LL(l, *this); };
	wstring PositionStr(Line l) 
	{
		switch (Position(l)) {
		case Parallel://平行
			return L"平行";
		case Coincidence://完全重合
			return L"完全重合";
		case PartCoincidence://部分重合
			return L"部分重合";
		case Vertical://垂直
			return L"垂直";
		case Intersect://相交
			return L"相交";
		case Disjoint://相离
			return L"相离";
		default:
			return L"其他";
		}
	};
};


class Base //封闭图形基类
{
private:
	float area(int a, int mode);
public:
	vector<Point> points;
	vector<Line> lines;

	Base();
	int Length();
	int AddPoint(Point p);//return index
	int AddPoint(float x, float y);//return index

	int UpdateLines();

	int SetPoint(Point p, int index);//return 1 success and 0 failure
	int SetPoint(float x, float y, int index);//return 1 success and 0 failure
	int SetPoint(int state, int index);//set state
	Point GetPoint(int index);//return point
	int DelPoint(int index);//返回剩余点数
	int DrawImage(SDL_Renderer *ren);
	int Update(int x,int y);
	virtual int JudgeShape() { return 0; };
	virtual wstring JudgeShapeStr() { return L""; };
	int JudgeConvex();//判断是否为凸多边形 1凸 2凹
	int Move(int x, int y);
	int MoveTo(int x, int y);

	float CalcDistance_PP(int i_p1, int i_p2);//点索引计算距离
	float CalcDistance_PL(int i_p1, int i_l_p1, int i_l_p2);//点索引计算点线距离
	
	float CalcArea(int mode=0);//mode 0屏幕像素  1坐标轴
	float CalcPerimeter(int mode=0);//mode 0屏幕像素  1坐标轴


	int cmp(float a, float b) { return (fabs(a - b) <= 0.1); };
	~Base();
};