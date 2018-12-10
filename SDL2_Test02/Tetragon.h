#pragma once
#include "Base.h"

enum Tetragon_Shape {
	Parallelogram,//平行四边形
	Diamond,//菱形
	Square,//正方形
	Rectangle,//矩形
	Trapezoid,//梯形
	IsoscelesTrapezoid,//等腰梯形
	GeneralTetragon,//一般四边形(凸)
	ConcaveTetragon//凹四边形
};

class Tetragon : virtual public Base
{
public:
	Tetragon();
	Tetragon(Point,Point,Point,Point);
	int JudgeShape_4();
	wstring JudgeShapeStr_4();
	~Tetragon();
};

