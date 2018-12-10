#include "Tetragon.h"


Tetragon::Tetragon() {

}
Tetragon::Tetragon(Point a, Point b, Point c, Point d)
{
	AddPoint(a);
	AddPoint(b);
	AddPoint(c);
	AddPoint(d);
	UpdateLines();
}
wstring Tetragon::JudgeShapeStr_4() {
	int n = JudgeShape();
	switch (n) {
	case Parallelogram:
		return L"平行四边形";
	case Diamond:
		return L"菱形";
	case Square:
		return L"正方形";
	case Rectangle:
		return L"矩形";
	case Trapezoid:
		return L"梯形";
	case IsoscelesTrapezoid:
		return L"等腰梯形";
	case GeneralTetragon:
		return L"一般四边形";
	case ConcaveTetragon:
		return L"凹四边形"; 
	default:
		return L"非四边形";
	}
}
int Tetragon::JudgeShape_4()
{
	if (Length() != 4)
		return -1;
	if (!JudgeConvex())//0凹  1凸
		return ConcaveTetragon;//凹四边形

	UpdateLines();
	//cout<< lines[0].Slope()<<" "<< lines[1].Slope()<<" "<< lines[2].Slope()<< " "<<lines[3].Slope()<<endl;
	if (lines[0].Slope() == lines[2].Slope() && lines[1].Slope() == lines[3].Slope()) {//平行四边形
		if (lines[0].Slope()*lines[1].Slope() == -1 && lines[1].Slope()*lines[2].Slope() == -1 && lines[2].Slope()*lines[3].Slope() == -1 || /*斜率之积为-1*/
			lines[0].Slope() == 999 && lines[1].Slope() == 0 && lines[2].Slope() == 999 && lines[3].Slope() == 0 || /*斜率为0和不存在*/
			lines[0].Slope() == 0 && lines[1].Slope() == 999 && lines[2].Slope() == 0 && lines[3].Slope() == 999)  //三个直角
		{
			if (lines[0].Length() == lines[1].Length() && lines[1].Length() == lines[2].Length() && lines[2].Length() == lines[3].Length())//四边相等
				return Square;//正方形
			return Rectangle;//矩形
		}
		else if (lines[0].Length() == lines[1].Length() && lines[1].Length() == lines[2].Length() && lines[2].Length() == lines[3].Length())//四边相等
		{
			return Diamond;//菱形
		}
		return Parallelogram;//平行四边形
	}
	else if(lines[0].Slope() == lines[2].Slope() || lines[1].Slope() == lines[3].Slope())//对边平行
	{
		if(lines[0].Slope() == lines[2].Slope() && lines[1].Length()== lines[3].Length() || lines[1].Slope() == lines[3].Slope() && lines[0].Length() == lines[2].Length())
			return IsoscelesTrapezoid;//等腰梯形

		return Trapezoid;//梯形
	}
	return GeneralTetragon;//四边形
}
Tetragon::~Tetragon()
{
}
