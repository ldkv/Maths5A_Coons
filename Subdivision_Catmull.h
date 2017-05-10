#pragma once

#include <vector>
#include <algorithm>
#include <random>

using namespace std;

static int globalId = 0;
static int globalSideId = 0;
static int globalFaceId = 0;

struct Point
{
	int id;
	QVector3D coord;
	vector<int> sides;
	Point()
	{
		id = -1;
	};

	Point(int idi, QVector3D pt, vector<int> s)
	{
		id = idi;
		coord = pt;
		sides = s;
	}
	Point(QVector3D pt)
	{
		coord = pt;
		id = globalId++;
	}

	bool operator==(const Point& e)
	{
		if (coord == e.coord)
			return true;

		return false;
	}
	Point operator*(int i)
	{
		this->coord *= i;
		return *this;
	}
};

Point computeBarycenter(vector<Point> points);

struct Side
{
	int id;
	int pLow;
	int pHigh;
	int fLeft;
	int fRight;

	vector<Point> points;
	int idFace1 = -1;
	int idFace2 = -1;

	Side()
	{
		pLow = -1;
		pHigh = -1;
		fLeft = -1;
		fRight = -1;
		id = -1;
	}

	Side(int l, int h)
	{
		pLow = l;
		pHigh = h;
		fLeft = -1;
		fRight = -1;
		id = globalSideId++;
	}
	Side(Point p1, Point p2) {
		points.clear();
		points.push_back(p1);
		points.push_back(p2);
		pLow = p1.id;
		pHigh = p2.id;
		fLeft = -1;
		fRight = -1;
		id = globalSideId++;
	}
	Side(Point p1, Point p2, int idFace) {
		points.clear();
		points.push_back(p1);
		points.push_back(p2);
		idFace1 = idFace;
		id = globalSideId++;
	}
	bool operator==(const Side& e)
	{
		if ((points[0].coord == e.points[0].coord && points[1].coord == e.points[1].coord) || (points[1].coord == e.points[0].coord && points[0].coord == e.points[1].coord))
			return true;

		return false;
	}
	bool operator!=(const Side& e) { return !(this->operator==(e)); }
};

struct Face
{
	int id;
	vector<Side> sides;
	vector<Point> points;
	Point barycenter;

	Face()
	{
		id = -1;
	}
	Face(vector<Point> _points) {
		points = _points;
		for (size_t i = 0; i < _points.size()-1; i++)
		{
			sides.push_back(Side(_points[i], _points[i+1]));
		}
		sides.push_back(Side(_points[_points.size()-1], _points[0]));
		id = globalFaceId++;
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
void deleteFacefromID(int id, vector<Face> &faces, vector<Side> &sides);
void deleteFacefromSide(vector<Side> &sides, int sideID, int faceID);
vector<Point> getVertexesfromFace(Face F, vector<Point> pts, vector<Side> sides);
int addSide(int p1, int p2, vector<Side> &sides, vector<Point> &pts);
int addFace(int id1, int id2, int id3, vector<Face> &faces, vector<Side> &sides);
void removeSidefromPoint(vector<Point> &pts, int ptID, int sideID);
void addSidetoPoint(vector<Point> &pts, int ptID, int sideID);
bool coordsSort(Point i, Point j);
vector<Side> getIncidentEdgesOriented(Point P, vector<Side> sides, vector<Point> pts);
vector<Face> getIncidentFacesOriented(vector<Side> La1, vector<Face> faces, vector<Side> sides, vector<Side> &La2);
int getPointIndex(vector<Point> pts, int id);
int getSideIDFromPoints(vector<Side> s, Point x, Point y);
int getPointIndex(vector<Point> pts, int id);


vector<Face> subdivide(vector<Face> faces);