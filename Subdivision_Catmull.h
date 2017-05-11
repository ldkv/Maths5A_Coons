#pragma once

#include <vector>
#include <algorithm>
#include <random>

using namespace std;

struct Point
{
	QVector3D coord;
	vector<int> sides;
	Point()
	{};

	Point(QVector3D pt)
	{
		coord = pt;
	}

	bool operator==(const Point& e)
	{
		if (coord == e.coord)
			return true;

		return false;
	}
	bool operator!=(const Point& e) { return !(this->operator==(e)); }
	Point operator*(int i)
	{
		this->coord *= i;
		return *this;
	}
};

struct Side
{
	vector<Point> points;

	Side()
	{}

	Side(Point p1, Point p2) {
		points.clear();
		points.push_back(p1);
		points.push_back(p2);
	}
	bool operator==(const Side& e)
	{
		if ((points[0].coord == e.points[0].coord && points[1].coord == e.points[1].coord) || (points[1].coord == e.points[0].coord && points[0].coord == e.points[1].coord))
			return true;

		return false;
	}
	bool operator!=(const Side& e) { return !(this->operator==(e)); }
};

Point computeBarycenter(vector<Point> points);

struct Face
{
	vector<Side> sides;
	vector<Point> points;
	Point barycenter;

	Face()
	{
	}
	Face(vector<Point> _points) {
		points = _points;
		for (size_t i = 0; i < _points.size()-1; i++)
		{
			sides.push_back(Side(_points[i], _points[i+1]));
		}
		sides.push_back(Side(_points[_points.size()-1], _points[0]));
		barycenter = computeBarycenter(_points);
	}
	bool operator==(const Face& e)
	{
		if (barycenter == e.barycenter)
			return true;

		return false;
	}
	bool operator!=(const Face& e) { return !(this->operator==(e)); }
};

void resetGlobalID();
Point getPointfromID(vector<Point> pts, int id);
Side getSidefromID(vector<Side> sds, int id);
Face getFacefromID(vector<Face> faces, int id);
int getSideIDfromPoints(vector<Side> sides, int p1, int p2);
void deleteSidefromID(int id, vector<Side> &sds, vector<Point> &pts);
void deleteSidefromPoint(vector<Point> &pts, int sideID, int pointID);
void deleteFacefromSide(vector<Side> &sides, int sideID, int faceID);
int addSide(int p1, int p2, vector<Side> &sides, vector<Point> &pts);
void removeSidefromPoint(vector<Point> &pts, int ptID, int sideID);
void addSidetoPoint(vector<Point> &pts, int ptID, int sideID);
vector<Side> getIncidentEdgesOriented(Point P, vector<Side> sides, vector<Point> pts);
int getPointIndex(vector<Point> pts, int id);
int getSideIDFromPoints(vector<Side> s, Point x, Point y);
int getPointIndex(vector<Point> pts, int id);


vector<Face> subdivideC(vector<Face> faces, int sizex = 4, int sizey = 4);