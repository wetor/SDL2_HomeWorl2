#include "Base.h"


float Calc::CalcDistance_PP(Point p1, Point p2) {
	return (sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)));
}
float Calc::CalcDistance_PL(Point p, Line l) {
	float k = l.Slope();
	float b = l.Intercept();
	float re = fabs(k * p.x - p.y + b);//|kx0-y0+b|
	re = re / (sqrt(k * k + 1.0));
	return re;
}
int Calc::JudgePosition_LL(Line l1, Line l2) {
	float k1 = l1.Slope();
	float b1 = l1.Intercept();
	float k2 = l2.Slope();
	float b2 = l2.Intercept();
	if (k1 == k2) {
		if (b1 != b2) {
			return Parallel; //平行
		}
		else {
			if (l1.point[0] == l2.point[0] && l1.point[1] == l2.point[1])
				return Coincidence; //完全重合
			else
				return PartCoincidence;//部分重合
		}
	}
	else {
		if (JudgePosition_LL_Segment(l1.point[0], l1.point[1], l2.point[0], l2.point[1])) {
			if (fabs(k1 * k2 + 1) <= 0.01 ||
				k1 == 999 && k2 == 0 ||
				k2 == 999 && k1 == 0) {
				return Vertical; //垂直
			}
			else {
				return Intersect; //相交
			}
		}
		else {
			return Disjoint; //相离
		}
		
	}
	return -1;


}
bool Calc::JudgePosition_LL_Segment(Point a, Point b, Point c, Point d)
{
	//https://www.cnblogs.com/wuwangchuxin0924/p/6218494.html
	/*
	   快速排斥：
	   两个线段为对角线组成的矩形，如果这两个矩形没有重叠的部分，那么两条线段是不可能出现重叠的
	   */
	if (!(min(a.x, b.x) <= max(c.x, d.x) && min(c.y, d.y) <= max(a.y, b.y) && min(c.x, d.x) <= max(a.x, b.x) && min(a.y, b.y) <= max(c.y, d.y)))//这里的确如此，这一步是判定两矩形是否相交
	//1.线段ab的低点低于cd的最高点（可能重合） 2.cd的最左端小于ab的最右端（可能重合）
	//3.cd的最低点低于ab的最高点（加上条件1，两线段在竖直方向上重合） 4.ab的最左端小于cd的最右端（加上条件2，两直线在水平方向上重合）
	//综上4个条件，两条线段组成的矩形是重合的
	/*特别要注意一个矩形含于另一个矩形之内的情况*/
		return false;
	/*
	跨立实验：
	如果两条线段相交，那么必须跨立，就是以一条线段为标准，另一条线段的两端点一定在这条线段的两段
	也就是说a b两点在线段cd的两端，c d两点在线段ab的两端
	*/
	double u, v, w, z;//分别记录两个向量
	u = (c.x - a.x)*(b.y - a.y) - (b.x - a.x)*(c.y - a.y);
	v = (d.x - a.x)*(b.y - a.y) - (b.x - a.x)*(d.y - a.y);
	w = (a.x - c.x)*(d.y - c.y) - (d.x - c.x)*(a.y - c.y);
	z = (b.x - c.x)*(d.y - c.y) - (d.x - c.x)*(b.y - c.y);
	return (u*v <= 0.0001 && w*z <= 0.0001);
}

Base::Base()
{
	points.clear();
	lines.clear();
}
int Base::Length()
{
	return points.size();

}
int Base::AddPoint(Point p)
{
	points.push_back(p);
	return points.size() - 1;

}
int Base::AddPoint(float x, float y)
{
	points.push_back(Point(x, y));
	return points.size() - 1;

}
int Base::SetPoint(Point p, int index)
{
	if (points.size() == 0 || index < 0 || index > points.size() - 1) {
		return 0;
	}
	points[index] = p;
	return 1;

}
int Base::SetPoint(float x, float y, int index)
{
	if (points.size() == 0 || index < 0 || index > points.size() - 1) {
		return 0;
	}
	points[index] = Point(x, y);
	return 1;

}
int Base::SetPoint(int state, int index)
{
	if (points.size() == 0 || index < 0 || index > points.size() - 1) {
		return 0;
	}
	points[index].state = state;
	return 1;

}

int Base::UpdateLines() {
	if (points.size() < 3) {
		return 0;
	}
	lines.clear();
	for (int i = 0; i < points.size()-1; i++) {
		lines.push_back(Line(points[i], points[i + 1]));
	}
	lines.push_back(Line(points[0], points[points.size() - 1]));
	return 1;
}

Point Base::GetPoint(int index)
{
	if (points.size() == 0 || index < 0 || index > points.size() - 1) {
		return Point();
	}
	return points[index];

}
int Base::DelPoint(int index)
{
	if (points.size() == 0 || index < 0 || index > points.size() - 1) {
		return 0;
	}
	points.erase(points.begin() + index);
	return points.size();
}
int Base::JudgeConvex() {
	//https://zhidao.baidu.com/question/514364714.html
	int n = points.size();
	int i, j, k;
	float z;
	if (n < 3)
		return(0);
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		k = (i + 2) % n;
		z = (points[j].x - points[i].x) * (points[k].y - points[j].y);
		z -= (points[j].y - points[i].y) * (points[k].x - points[j].x);
		if (z > 0)
			return 0;
	}
	return 1;
}
int Base::DrawImage(SDL_Renderer *ren) {
	if (points.size() == 0) {
		return 0;
	}
	
	if (points.size() > 1) {
		SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 0xFF);//蓝色 线
		for (int i = 0; i < points.size() - 1; i++) {//画线
			SDL_RenderDrawLine(ren, (int)points[i].x, (int)points[i].y, (int)points[i+1].x, (int)points[i+1].y);
		}
		SDL_RenderDrawLine(ren, (int)points[0].x, (int)points[0].y, (int)points[points.size()-1].x, (int)points[points.size()-1].y);
	}
	SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0xFF);//红色 点
	for (int i = 0; i < points.size(); i++) {//画点
		if (points[i].state != Mouse_None) {
			SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 0xFF);
			Func::DrawCircle(ren, (int)points[i].x, (int)points[i].y, point_r + 2);//焦点
			SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0xFF);//红色 点
		}
		Func::DrawCircle(ren, (int)points[i].x, (int)points[i].y, point_r);//正常
	}
	return 1;
}
int Base::Update(int x,int y) {
	if (points.size() == 0) {
		return -1;
	}
	for (int i = 0; i < points.size(); i++) {
		if (points[i].state == Mouse_Press) {
			points[i] = Point(x, y);
			points[i].state = Mouse_Press;
			return i;
		}
		else {
			if (x >= points[i].rect.x && y >= points[i].rect.y &&
				x <= points[i].rect.x + points[i].rect.w&& y <= points[i].rect.y + points[i].rect.h)
			{
				points[i].state = Mouse_Focus;
				return i;
			}
			else {
				points[i].state = Mouse_None;
			}
		}
	}
		
	
	return -1;
}

int Base::Move(int x, int y) {
	if (points.size() == 0) {
		return 0;
	}
	for (int i = 0; i < points.size(); i++) {
		if(points[i].state != Mouse_Press)
			points[i] = Point(points[i].x + x, points[i].y + y);
	}
	return 1;
}
int Base::MoveTo(int x, int y) {
	if (points.size() == 0) {
		return 0;
	}
	for (int i = 0; i < points.size(); i++) {
		if (points[i].state != Mouse_Press)
			points[i] = Point(x, y);
	}
	return 1;
}
float Base::CalcDistance_PP(int i_p1, int i_p2) {
	if (points.size() == 0 || i_p1 < 0 || i_p1 > points.size() - 1 || i_p2 < 0 || i_p2 > points.size() - 1) {
		return 0;
	}
	return Calc::CalcDistance_PP(points[i_p1], points[i_p2]);
}

float Base::CalcDistance_PL(int i_p1, int i_l_p1, int i_l_p2) {

	if (points.size() == 0 || i_p1 < 0 || i_p1 > points.size() - 1 || i_l_p1 < 0 || i_l_p1 > points.size() - 1 || i_l_p2 < 0 || i_l_p2 > points.size() - 1) {
		return 0;
	}

	return Calc::CalcDistance_PL(points[i_p1], Line(points[i_l_p1], points[i_l_p2]));
}
float Base::area(int a, int mode = 0)
{
	int b = a - 1;
	return (points[b].x*points[a].y - points[a].x*points[b].y) - (points[0].x*points[a].y - points[a].x*points[0].y) + (points[0].x*points[b].y - points[b].x*points[0].y);
}
float Base::CalcArea(int mode) {
	//https://blog.csdn.net/tigercoder/article/details/70161646
	if (points.size() <= 2) {
		return 0;
	}
	float sum = 0;
	for (int i = 2; i < points.size(); i++) {
		sum += 1.0f / 2.0f*area(i,mode);
	}
	if (mode)
		sum = sum / 16 / 16;
	return fabs(sum);
}
float Base::CalcPerimeter(int mode) {
	if (points.size() <= 2) {
		return 0;
	}
	float sum = 0;
	for (int i = 0; i < points.size()-1; i++) {
		sum += Line(points[i], points[i + 1]).Length();
	}
	sum += Line(points[0], points[points.size() - 1]).Length();

	if (mode)
		sum = sum / 16;
	//cout << "len:" << Line(0,0,10,10).Length() << endl;
	return sum;

}
Base::~Base()
{
	points.clear();
}



