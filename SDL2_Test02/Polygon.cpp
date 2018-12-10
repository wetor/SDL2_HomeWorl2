#include "Polygon.h"


Polygon::Polygon(int count, ...) {
	//https://www.cnblogs.com/litmin/p/7508682.html
	if (count <= 0)
		return ;
	va_list arg_ptr;
	va_start(arg_ptr, count);
	for (int i = 0; i < count; i++)
	{
		AddPoint(va_arg(arg_ptr, Point));
	}
	UpdateLines();
	va_end(arg_ptr);

}

int Polygon::JudgeShape() {
	if (Length() == 3)
		return JudgeShape_3();
	else if(Length() == 4)
		return JudgeShape_4();
	else
		return Length();
}

wstring Polygon::JudgeShapeStr() {


	if (Length() <= 0)
		return L"无图像";
	wchar_t wstr[256];
	switch (Length()) {
	case 1:
		return L"点";
	case 2:
		return L"线"; 
	case 3:
		return JudgeShapeStr_3();
	case 4:
		return JudgeShapeStr_4();
	case 5:
		return L"五边形";
	case 6:
		return L"六边形";
	case 7:
		return L"七边形";
	case 8:
		return L"八边形";
	default:
		wprintf(wstr, L"%d边形", Length());
		return wstr;

	}


}
Polygon::~Polygon()
{

}
