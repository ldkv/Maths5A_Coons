#pragma once

#include <vector>
#include <QVector3D>

#define MAX_EDGES_ADJACENT 100
#define PI 3.14159265358979

using namespace std;

struct Edge;
struct Triangle;

struct Vertex
{
	QVector3D coord;
	vector<Edge*> vEs;

	Vertex(QVector3D pt, vector<Edge*> ver)
	{
		coord = pt;
		vEs = ver;
	}

	Vertex(QVector3D pt)
	{
		coord = pt;
	}

	Vertex(double x, double y, double z)
	{
		coord = QVector3D(x, y, z);
	}
};

struct Edge
{
	Vertex* v1 = nullptr;
	Vertex* v2 = nullptr;
	Triangle* t1 = nullptr;
	Triangle* t2 = nullptr;
	QVector3D edgePt;

	Edge(Vertex *p1, Vertex *p2)
	{
		v1 = p1;
		v2 = p2;
	}

	void addTriangle(Triangle *t)
	{
		if (t1 == nullptr)
			t1 = t;
		else
			t2 = t;
	}
};

struct Triangle
{
	vector<Vertex*> tVs;
	vector<Edge*> tEs;
	
	Triangle(Vertex *p1, Vertex *p2, Vertex *p3)
	{
		tVs.clear();
		tVs.push_back(p1);
		tVs.push_back(p2);
		tVs.push_back(p3);
	}
};

void calcAlphaLoop();
void createCube(vector<Triangle*>&, vector<Edge*>&, vector<Vertex*>&);
void addTriangle(Vertex*, Vertex*, Vertex*, vector<Triangle*>&, vector<Edge*>&);
Edge* getEdgefromVertexes(vector<Edge*>, Vertex*, Vertex*);
Edge* updateEdge(vector<Edge*>&, Vertex*, Vertex*, Triangle*);
Vertex* getOtherVertexfromEdge(Vertex*, Edge*);
Vertex* getOtherVertexfromTriangle(Edge*, Triangle*);
void Subdivision_Loop(vector<Triangle*>&, vector<Edge*>&, vector<Vertex*>&);