#include "Triangle.h"

Triangle::Triangle() {

}

Triangle::Triangle(Point a, Point b, Point c)
{
	AddPoint(a);
	AddPoint(b);
	AddPoint(c);
	UpdateLines();
}
int Triangle::Judge() {

	if (Length() != 3)
		return 0;
	UpdateLines();
	if (lines[0].Length() + lines[1].Length() >= lines[2].Length() && lines[0].Length() + lines[2].Length() >= lines[1].Length() && lines[1].Length() + lines[2].Length() >= lines[0].Length()) {
		return 1;
	}
	return 0;
}
wstring Triangle::JudgeShapeStr_3() {
	int n = JudgeShape();
	switch (n) {
	case Equilateral:
		return L"��������";
	case Isosceles:
		return L"����������";
	case Right:
		return L"ֱ��������";
	case IsoscelesRight:
		return L"����ֱ��������";
	case GeneralTriangle:
		return L"һ��������";
	default:
		return L"��������";
	}
}
int Triangle::JudgeShape_3()
{
	if (!Judge())
		return -1;
	int a=0,b=0,c=0;
	int re;
	if (lines[0].Length() == lines[1].Length() && lines[1].Length() == lines[2].Length()) {
		re = Equilateral;
		a = 1;
	}
	if (lines[0].Length() == lines[1].Length() && lines[0].Length() != lines[2].Length() ||
		lines[0].Length() == lines[2].Length() && lines[0].Length() != lines[1].Length() || 
		lines[1].Length() == lines[2].Length() && lines[1].Length() != lines[0].Length() ) {
		re = Isosceles;
		b = 1;
	}
	if (cmp(lines[0].Length()*lines[0].Length()+lines[1].Length()*lines[1].Length(),lines[2].Length()*lines[2].Length()) ||
		cmp(lines[0].Length()*lines[0].Length()+lines[2].Length()*lines[2].Length(),lines[1].Length()*lines[1].Length()) ||
		cmp(lines[1].Length()*lines[1].Length()+lines[2].Length()*lines[2].Length(),lines[0].Length()*lines[0].Length())) {
		re = Right;
		c = 1;
	}
	if (b && c)
		return IsoscelesRight;
	else if(a||b||c)
		return re;
	return GeneralTriangle;

}

Triangle::~Triangle()
{
}
