#pragma once
#include "Base.h"
enum Triangle_Shape {
	Equilateral,//�ȱ�������
	Isosceles,//����������
	Right,//ֱ��������
	IsoscelesRight,//����ֱ��������
	GeneralTriangle//һ��������

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

