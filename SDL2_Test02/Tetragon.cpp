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
		return L"ƽ���ı���";
	case Diamond:
		return L"����";
	case Square:
		return L"������";
	case Rectangle:
		return L"����";
	case Trapezoid:
		return L"����";
	case IsoscelesTrapezoid:
		return L"��������";
	case GeneralTetragon:
		return L"һ���ı���";
	case ConcaveTetragon:
		return L"���ı���"; 
	default:
		return L"���ı���";
	}
}
int Tetragon::JudgeShape_4()
{
	if (Length() != 4)
		return -1;
	if (!JudgeConvex())//0��  1͹
		return ConcaveTetragon;//���ı���

	UpdateLines();
	//cout<< lines[0].Slope()<<" "<< lines[1].Slope()<<" "<< lines[2].Slope()<< " "<<lines[3].Slope()<<endl;
	if (lines[0].Slope() == lines[2].Slope() && lines[1].Slope() == lines[3].Slope()) {//ƽ���ı���
		if (lines[0].Slope()*lines[1].Slope() == -1 && lines[1].Slope()*lines[2].Slope() == -1 && lines[2].Slope()*lines[3].Slope() == -1 || /*б��֮��Ϊ-1*/
			lines[0].Slope() == 999 && lines[1].Slope() == 0 && lines[2].Slope() == 999 && lines[3].Slope() == 0 || /*б��Ϊ0�Ͳ�����*/
			lines[0].Slope() == 0 && lines[1].Slope() == 999 && lines[2].Slope() == 0 && lines[3].Slope() == 999)  //����ֱ��
		{
			if (lines[0].Length() == lines[1].Length() && lines[1].Length() == lines[2].Length() && lines[2].Length() == lines[3].Length())//�ı����
				return Square;//������
			return Rectangle;//����
		}
		else if (lines[0].Length() == lines[1].Length() && lines[1].Length() == lines[2].Length() && lines[2].Length() == lines[3].Length())//�ı����
		{
			return Diamond;//����
		}
		return Parallelogram;//ƽ���ı���
	}
	else if(lines[0].Slope() == lines[2].Slope() || lines[1].Slope() == lines[3].Slope())//�Ա�ƽ��
	{
		if(lines[0].Slope() == lines[2].Slope() && lines[1].Length()== lines[3].Length() || lines[1].Slope() == lines[3].Slope() && lines[0].Length() == lines[2].Length())
			return IsoscelesTrapezoid;//��������

		return Trapezoid;//����
	}
	return GeneralTetragon;//�ı���
}
Tetragon::~Tetragon()
{
}
