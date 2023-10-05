#pragma once
// transform paramater
struct Asc
{
	Asc(double xcorner, double ycorner, double dx,double rows,double cols) {
		this->xcorner = xcorner;
		this->ycorner = ycorner;
		this->dx = dx;
		this->rows = rows;
		this->cols = cols;
	}
	double xcorner, ycorner, dx,rows,cols;
};

//absolute point
template < typename T >
struct Point2D
{
	Point2D<T>() {}
	Point2D<T>(T X, T Y) {
		this->x = X;
		this->y = Y;
	}
	T x,y;
};

//abstract space
template < typename T >
class space
{
public:
	virtual Point2D<T> position(int index)=0;
private:

};

//RelativeSpace
class RelativeSpace:public space<int>
{
public:
	RelativeSpace();
	RelativeSpace(Asc asc, Point2D<double> point);
	RelativeSpace(Asc asc, vector<Point2D<double>>& point);
	~RelativeSpace();
	Point2D<int> position(int index);
	int Row(int i){
		return (int)points[i].x;
	}
	int Col(int i) {
		return (int)points[i].y;
	}
	int Count() {
		return points.size();
	}
private:
	vector<Point2D<int>> points;
};
RelativeSpace::RelativeSpace()
{
	printf("没想好写啥");
}
RelativeSpace::RelativeSpace(Asc asc, Point2D<double> point)
{
	Point2D<int> p;
	p.x = floor((point.x - asc.xcorner) / asc.dx);
	p.y = floor((point.y - asc.ycorner) / asc.dx);
	points.push_back(p);
}
RelativeSpace::RelativeSpace(Asc asc, vector<Point2D<double>>& point)
{
	if (points.empty()&&!point.empty()) {
		for (int i = 0; i < point.size(); i++)
		{
			Point2D<int> p;
			p.x = floor((point[i].x - asc.xcorner) / asc.dx);
			p.y = floor((point[i].y - asc.ycorner) / asc.dx);
			if (point[i].x >= asc.xcorner && point[i].y >= asc.xcorner && point[i].x < asc.xcorner + asc.dx * asc.cols && point[i].y < asc.ycorner + asc.dx * asc.cols) {
				points.push_back(p);
			}		
		}
		point.clear();
		vector<Point2D<double>>(point).swap(point);
	}
}
Point2D<int> RelativeSpace::position(int index) {
	return points[index];
}
RelativeSpace::~RelativeSpace()
{
	printf("没想好写啥");
}
//Decorator to feng zhuang water swap
class spaceDecorator :space<int> 
{
public:
	spaceDecorator(RelativeSpace& space);
	Point2D<int> position(int index) { Point2D<int> P(0,0); return P; }
	void loop() { printf("没想好写啥"); }

private:
	space<int>* re_sp;
};
spaceDecorator::spaceDecorator(RelativeSpace& space)
{
	this->re_sp = &space;
}