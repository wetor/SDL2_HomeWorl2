#pragma once
#include "Base.h"
enum Triangle_Shape {
	Equilateral,//等边三角形
	Isosceles,//等腰三角形
	Right,//直角三角形
	IsoscelesRight,//等腰直角三角形
	GeneralTriangle//一般三角形

};
class Triangle :virtual public Base
{
public:
	Triangle();
	Triangle(Point a,Point b,Point c);
	int JudgeShape_3();
	wstring JudgeShapeStr_3();
	int Judge();
	~Triangle();
};

