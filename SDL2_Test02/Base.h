#pragma once

#include <vector>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "Func.h"


using namespace std;

enum State {
	Mouse_None,//һ��
	Mouse_Focus,//����
	Mouse_Press//����
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
	float x, y;//��Ļ���꣬����
	int ax, ay;//�����ᣬ��λ
	SDL_Rect rect;
	int state;
	bool operator==(const Point& b) {
		return (b.x == this->x&&b.y == this->y);
	}
};

enum Position {
	Parallel,//ƽ��
	Coincidence, //��ȫ�غ�
	PartCoincidence, //�����غ�
	Vertical,//��ֱ
	Intersect,//�ཻ
	Disjoint//����
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
	};//б��
	float Intercept() { 
		if (Slope() == 999)
			return 0;
		else 
			return point[0].y - Slope() * point[0].x;
	};//�ؾ�

	float Length() { return  Calc::CalcDistance_PP(point[0], point[1]); };//����
	float DistanceToPoint(Point p) { return Calc::CalcDistance_PL(p, *this); };
	float DistanceToPoint(float x, float y) { return Calc::CalcDistance_PL(Point(x, y), *this); };
	int Position(Line l) { return Calc::JudgePosition_LL(l, *this); };
	wstring PositionStr(Line l) 
	{
		switch (Position(l)) {
		case Parallel://ƽ��
			return L"ƽ��";
		case Coincidence://��ȫ�غ�
			return L"��ȫ�غ�";
		case PartCoincidence://�����غ�
			return L"�����غ�";
		case Vertical://��ֱ
			return L"��ֱ";
		case Intersect://�ཻ
			return L"�ཻ";
		case Disjoint://����
			return L"����";
		default:
			return L"����";
		}
	};
};


class Base //���ͼ�λ���
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
	int DelPoint(int index);//����ʣ�����
	int DrawImage(SDL_Renderer *ren);
	int Update(int x,int y);
	virtual int JudgeShape() { return 0; };
	virtual wstring JudgeShapeStr() { return L""; };
	int JudgeConvex();//�ж��Ƿ�Ϊ͹����� 1͹ 2��
	int Move(int x, int y);
	int MoveTo(int x, int y);

	float CalcDistance_PP(int i_p1, int i_p2);//�������������
	float CalcDistance_PL(int i_p1, int i_l_p1, int i_l_p2);//������������߾���
	
	float CalcArea(int mode=0);//mode 0��Ļ����  1������
	float CalcPerimeter(int mode=0);//mode 0��Ļ����  1������


	int cmp(float a, float b) { return (fabs(a - b) <= 0.1); };
	~Base();
};