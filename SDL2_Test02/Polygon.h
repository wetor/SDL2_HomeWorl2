#pragma once
#include "Base.h"
#include "Triangle.h"
#include "Tetragon.h"
#include "stdarg.h"
class Polygon : public Triangle , public Tetragon
{
public:
	Polygon(int count, ...);
	int JudgeShape();
	wstring JudgeShapeStr();

	~Polygon();
};

