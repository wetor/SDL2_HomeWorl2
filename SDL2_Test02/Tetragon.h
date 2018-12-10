#pragma once
#include "Base.h"

enum Tetragon_Shape {
	Parallelogram,//ƽ���ı���
	Diamond,//����
	Square,//������
	Rectangle,//����
	Trapezoid,//����
	IsoscelesTrapezoid,//��������
	GeneralTetragon,//һ���ı���(͹)
	ConcaveTetragon//���ı���
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

