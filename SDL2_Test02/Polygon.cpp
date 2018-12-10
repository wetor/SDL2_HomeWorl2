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
		return L"��ͼ��";
	wchar_t wstr[256];
	switch (Length()) {
	case 1:
		return L"��";
	case 2:
		return L"��"; 
	case 3:
		return JudgeShapeStr_3();
	case 4:
		return JudgeShapeStr_4();
	case 5:
		return L"�����";
	case 6:
		return L"������";
	case 7:
		return L"�߱���";
	case 8:
		return L"�˱���";
	default:
		wprintf(wstr, L"%d����", Length());
		return wstr;

	}


}
Polygon::~Polygon()
{

}
