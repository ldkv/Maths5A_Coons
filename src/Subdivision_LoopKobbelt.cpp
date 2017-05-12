#include "stdafx.h"
#include "Subdivision_LoopKobbelt.h"


void addTriangle(Vertex* p1, Vertex* p2, Vertex* p3, vector<Triangle*> &ts, vector<Edge*> &es)
{
	Triangle* t = new Triangle(p1, p2, p3);
	t->tEs.push_back(updateEdge(es, p2, p3, t));
	t->tEs.push_back(updateEdge(es, p3, p1, t));
	t->tEs.push_back(updateEdge(es, p1, p2, t));
	ts.push_back(t);
}

Edge* updateEdge(vector<Edge*> &es, Vertex* p1, Vertex* p2, Triangle* t)
{
	int i = 0;
	for (i = 0; i < es.size(); i++)
	{
		if ((es[i]->v1 == p1 && es[i]->v2 == p2)
			|| (es[i]->v1 == p2 && es[i]->v2 == p1))
		{
			es[i]->addTriangle(t);
			return es[i];
		}
	}

	Edge *e = new Edge(p1, p2);
	e->addTriangle(t);
	es.push_back(e);
	es[i]->v1->vEs.push_back(es[i]);
	es[i]->v2->vEs.push_back(es[i]);
	return e;
}

void updateVertex(Vertex *v, Edge *e)
{
	int i = 0;
	for (i = 0; i < v->vEs.size(); i++)
	{
		if (v->vEs[i] == e)
			break;
	}
	if (i == v->vEs.size())
		v->vEs.push_back(e);
}

Edge* getEdgefromVertexes(vector<Edge*> es, Vertex *p1, Vertex *p2)
{
	for (int i = 0; i < es.size(); i++)
	{
		if ((es[i]->v1 == p1 && es[i]->v2 == p2)
			|| (es[i]->v1 == p2 && es[i]->v2 == p1))
			return es[i];
	}
	return nullptr;
}

void createCube(vector<Triangle*> &ts, vector<Edge*> &es, vector<Vertex*> &vs)
{
	calcAlphaLoop();
	calcAlphaKobbelt();

	Vertex* p000 = new Vertex(0, 0, 0);
	Vertex* p002 = new Vertex(0, 0, 2);
	Vertex* p020 = new Vertex(0, 2, 0);
	Vertex* p200 = new Vertex(2, 0, 0);
	Vertex* p202 = new Vertex(2, 0, 2);
	Vertex* p220 = new Vertex(2, 2, 0);
	Vertex* p222 = new Vertex(2, 2, 2);
	Vertex* p022 = new Vertex(0, 2, 2);
	Vertex* p110 = new Vertex(1, 1, 0);
	Vertex* p101 = new Vertex(1, 0, 1);
	Vertex* p011 = new Vertex(0, 1, 1);
	Vertex* p211 = new Vertex(2, 1, 1);
	Vertex* p121 = new Vertex(1, 2, 1);
	Vertex* p112 = new Vertex(1, 1, 2);
	vs.push_back(p000);
	vs.push_back(p002);
	vs.push_back(p020);
	vs.push_back(p200);
	vs.push_back(p202);
	vs.push_back(p220);
	vs.push_back(p222);
	vs.push_back(p022);
	vs.push_back(p110);
	vs.push_back(p101);
	vs.push_back(p011);
	vs.push_back(p211);
	vs.push_back(p121);
	vs.push_back(p112);

	for each (Vertex* v in vs)
	{
		v->coord *= 10;
	}

	addTriangle(p000, p200, p110, ts, es);
	addTriangle(p200, p110, p220, ts, es);
	addTriangle(p110, p220, p020, ts, es);
	addTriangle(p110, p020, p000, ts, es);

	addTriangle(p000, p200, p101, ts, es);
	addTriangle(p200, p101, p202, ts, es);
	addTriangle(p202, p101, p002, ts, es);
	addTriangle(p101, p002, p000, ts, es);
	
	addTriangle(p000, p020, p011, ts, es);
	addTriangle(p020, p011, p022, ts, es);
	addTriangle(p022, p011, p002, ts, es);
	addTriangle(p011, p002, p000, ts, es);

	addTriangle(p002, p202, p112, ts, es);
	addTriangle(p202, p112, p222, ts, es);
	addTriangle(p112, p222, p022, ts, es);
	addTriangle(p112, p022, p002, ts, es);

	addTriangle(p020, p220, p121, ts, es);
	addTriangle(p220, p121, p222, ts, es);
	addTriangle(p222, p121, p022, ts, es);
	addTriangle(p121, p022, p020, ts, es);

	addTriangle(p200, p220, p211, ts, es);
	addTriangle(p220, p211, p222, ts, es);
	addTriangle(p222, p211, p202, ts, es);
	addTriangle(p211, p202, p200, ts, es);
}

double alphaLoop[MAX_EDGES_ADJACENT];

void calcAlphaLoop()
{
	for (int n = 1; n < MAX_EDGES_ADJACENT; n++)
	{
		double n_inv = 1.0 / n;
		double cos2 = 3.0 / 8.0 + 1.0 / 4.0 * cos(2*PI*n_inv);
		cos2 *= cos2;
		alphaLoop[n] = n_inv * (5.0 / 8.0 - cos2);
	}
}

Vertex* getOtherVertexfromEdge(Vertex* v, Edge* e)
{
	if (e->v1 == v)
		return e->v2;
	return e->v1;
}

Vertex* getOtherVertexfromTriangle(Edge* e, Triangle* t)
{
	if (t == nullptr || e == nullptr)
		return nullptr;
	for each (Vertex* v in t->tVs)
		if (v != e->v1 && v != e->v2)
			return v;
	return nullptr;
}

void removeEdgefromVertex(Edge* e, Vertex* v)
{
	int i;
	for (i = 0; i < v->vEs.size(); i++)
		if (v->vEs[i] == e)
			break;
	if (i < v->vEs.size())
		v->vEs.erase(v->vEs.begin() + i);
}

void Subdivision_Loop(vector<Triangle*> &ts, vector<Edge*> &es, vector<Vertex*> &vs)
{
	// Calculer les edge points
	for (int i = 0; i < es.size(); i++)
	{
		es[i]->edgePt = es[i]->v1->coord + es[i]->v2->coord;
		if (es[i]->t2 == nullptr)
			es[i]->edgePt = es[i]->edgePt / 2.0;
		else
		{
			QVector3D vLeft = getOtherVertexfromTriangle(es[i], es[i]->t1)->coord;
			QVector3D vRight = getOtherVertexfromTriangle(es[i], es[i]->t2)->coord;
			es[i]->edgePt = 1.0 / 8.0 *(3 * es[i]->edgePt + vLeft + vRight);
		}
	}

	// Calculer des nouveaux vertexes
	vector<QVector3D> vtemp;
	for (int i = 0; i < vs.size(); i++)
	{
		int n = vs[i]->vEs.size();
		QVector3D somme(0, 0, 0);
		for each (Edge* e in vs[i]->vEs)
			somme += getOtherVertexfromEdge(vs[i], e)->coord;

		vtemp.push_back((1 - n*alphaLoop[n]) * vs[i]->coord + alphaLoop[n] * somme);
	}
	for (int i = 0; i < vs.size(); i++)
		vs[i]->coord = vtemp[i];


	int oldTriangleSize = ts.size();
	int oldEdgeSize = es.size();
	// Calculer les nouveaux triangles
	for (int i = 0; i < oldTriangleSize; i++)
	{
		Vertex *eP0 = new Vertex(ts[i]->tEs[0]->edgePt);
		Vertex *eP1 = new Vertex(ts[i]->tEs[1]->edgePt);
		Vertex *eP2 = new Vertex(ts[i]->tEs[2]->edgePt);
		vs.push_back(eP0);
		vs.push_back(eP1);
		vs.push_back(eP2);
		addTriangle(ts[i]->tVs[0], eP1, eP2, ts, es);
		addTriangle(ts[i]->tVs[1], eP0, eP2, ts, es);
		addTriangle(ts[i]->tVs[2], eP0, eP1, ts, es);
		addTriangle(eP0, eP1, eP2, ts, es);
	}
	// Supprimer les ancients edges
	for (int i = 0; i < oldEdgeSize; i++)
	{
		removeEdgefromVertex(es[i], es[i]->v1);
		removeEdgefromVertex(es[i], es[i]->v2);
		delete es[i];
	}
	es.erase(es.begin(), es.begin() + oldEdgeSize);

	// Supprimer les ancients triangles
	for (int i = 0; i < oldTriangleSize; i++)
		delete ts[i];
	ts.erase(ts.begin(), ts.begin() + oldTriangleSize);
	oldEdgeSize = 0;
}

double alphaKobbelt[MAX_EDGES_ADJACENT];

void calcAlphaKobbelt()
{
	for (int n = 1; n < MAX_EDGES_ADJACENT; n++)
		alphaKobbelt[n] = (4 - 2 * cos(2.0 * PI / n)) / 9.0;
}

Edge* newEdge(vector<Edge*> &es, Vertex* p1, Vertex* p2, Triangle* t)
{
	Edge *e = new Edge(p1, p2);
	e->addTriangle(t);
	e->v1->vEs.push_back(e);
	e->v2->vEs.push_back(e);
	es.push_back(e);
	return e;
}

void addTriangleWithEdge(Vertex* p1, Vertex* p2, Vertex* p3, vector<Triangle*> &ts, vector<Edge*> &es)
{
	Triangle* t = new Triangle(p1, p2, p3);
	t->tEs.push_back(newEdge(es, p2, p3, t));
	t->tEs.push_back(updateEdge(es, p3, p1, t));
	t->tEs.push_back(updateEdge(es, p1, p2, t));
	ts.push_back(t);
}

void Subdivision_Kobbelt(vector<Triangle*> &ts, vector<Edge*> &es, vector<Vertex*> &vs)
{
	// Calculer des nouveaux vertexes
	vector<QVector3D> vtemp;
	for (int i = 0; i < vs.size(); i++)
	{
		int n = vs[i]->vEs.size();
		QVector3D somme(0, 0, 0);
		for each (Edge* e in vs[i]->vEs)
			somme += getOtherVertexfromEdge(vs[i], e)->coord;

		vtemp.push_back((1 - alphaKobbelt[n]) * vs[i]->coord + alphaKobbelt[n] * somme / n);
	}

	for each (Triangle* t in ts)
	{
		t->barycenter = new Vertex((t->tVs[0]->coord + t->tVs[1]->coord + t->tVs[2]->coord) / 3.0);
		vs.push_back(t->barycenter);
	}

	for (int i = 0; i < vtemp.size(); i++)
		vs[i]->coord = vtemp[i];

	int oldTriangleSize = ts.size();
	int oldEdgeSize = es.size();
	// Calculer les nouveaux triangles
	for (int i = 0; i < oldTriangleSize; i++)
	{
		for each (Edge* e in ts[i]->tEs)
		{
			if (e->t1 != nullptr && e->t2 != nullptr)
			{
				if (e->t1 != ts[i])
				{
					addTriangle(ts[i]->barycenter, e->t1->barycenter, e->v1, ts, es);
					addTriangle(ts[i]->barycenter, e->t1->barycenter, e->v2, ts, es);
				}
				else
				{
					addTriangle(ts[i]->barycenter, e->t2->barycenter, e->v1, ts, es);
					addTriangle(ts[i]->barycenter, e->t2->barycenter, e->v2, ts, es);
				}
			}
			else if (e->t1 != nullptr || e->t2 != nullptr)
			{
				addTriangleWithEdge(ts[i]->barycenter, e->v1, e->v2, ts, es);
			}
			e->t1 = nullptr;
			e->t2 = nullptr;
		}
	}

	// Supprimer les ancients edges
	for (int i = 0; i < oldEdgeSize; i++)
	{
		removeEdgefromVertex(es[i], es[i]->v1);
		removeEdgefromVertex(es[i], es[i]->v2);
		delete es[i];
	}
	es.erase(es.begin(), es.begin() + oldEdgeSize);

	// Supprimer les ancients triangles
	for (int i = 0; i < oldTriangleSize; i++)
		delete ts[i];
	ts.erase(ts.begin(), ts.begin() + oldTriangleSize);
	oldEdgeSize = 0;
}

void Subdivision_Butterfly(vector<Triangle*> &ts, vector<Edge*> &es, vector<Vertex*> &vs)
{
	// Calculer les edge points
	for (int i = 0; i < es.size(); i++)
	{
		QVector3D v1 = es[i]->v1->coord;
		QVector3D v2 = es[i]->v2->coord;
		QVector3D vL, vR, vLL, vLR, vRL, vRR;
		bool testVL = false, testVR = false, testVLL = false, testVLR = false, testVRL = false, testVRR = false;
		Vertex* vtemp, *vtemp2;
		Edge* etemp;
		
		if (es[i]->t1 != nullptr)
		{
			vtemp = getOtherVertexfromTriangle(es[i], es[i]->t1);
			vL = vtemp->coord;
			testVL = true;
			etemp = getEdgefromVertexes(es, es[i]->v1, vtemp);
			vtemp2 = getOtherVertexfromTriangle(etemp, etemp->t1 != es[i]->t1 ? etemp->t1 : etemp->t2);
			if (vtemp2 != nullptr)
			{
				vLL = vtemp2->coord;
				testVLL = true;
			}
			etemp = getEdgefromVertexes(es, es[i]->v2, vtemp);
			vtemp2 = getOtherVertexfromTriangle(etemp, etemp->t1 != es[i]->t1 ? etemp->t1 : etemp->t2);
			if (vtemp2 != nullptr)
			{
				vLR = vtemp2->coord;
				testVLR = true;
			}
		}

		if (es[i]->t2 != nullptr)
		{
			vtemp = getOtherVertexfromTriangle(es[i], es[i]->t2);
			vR = vtemp->coord;
			testVR = true;
			etemp = getEdgefromVertexes(es, es[i]->v1, vtemp);
			vtemp2 = getOtherVertexfromTriangle(etemp, etemp->t1 != es[i]->t2 ? etemp->t1 : etemp->t2);
			if (vtemp2 != nullptr)
			{
				vRL = vtemp2->coord;
				testVRL = true;
			}
			etemp = getEdgefromVertexes(es, es[i]->v2, vtemp);
			vtemp2 = getOtherVertexfromTriangle(etemp, etemp->t1 != es[i]->t2 ? etemp->t1 : etemp->t2);
			if (vtemp2 != nullptr)
			{
				vRR = vtemp2->coord;
				testVRR = true;
			}
		}

		if (testVL && testVR && testVLL && testVLR && testVRR && testVRL)
		{
			es[i]->edgePt = 0.5*(v1 + v2) + 0.125*(vL + vR) - 0.0625*(vLL + vLR + vRL + vRR);
			continue;
		}
		if (testVL && testVR && testVRL && testVRR && testVLR && !testVLL)
		{
			es[i]->edgePt = 0.375*v1 + 0.625*v2 + 0.0625*vL + 0.1875*vR - 0.0625*(2 * vRR + vLR + vRL);
			continue;
		}
		if (testVL && testVR && testVRL && testVRR && testVLL && !testVLR)
		{
			es[i]->edgePt = 0.375*v2 + 0.625*v1 + 0.0625*vL + 0.1875*vR - 0.0625*(2 * vRL + vLL + vRR);
			continue;
		}
		if (testVL && testVR && testVLL && testVLR && testVRR && !testVRL)
		{
			es[i]->edgePt = 0.375*v1 + 0.625*v2 + 0.0625*vR + 0.1875*vL - 0.0625*(2 * vLR + vLL + vRR);
			continue;
		}
		if (testVL && testVR && testVLL && testVLR && testVRL && !testVRR)
		{
			es[i]->edgePt = 0.375*v2 + 0.625*v1 + 0.0625*vR + 0.1875*vL - 0.0625*(2 * vLL + vLR + vRL);
			continue;
		}
		if (testVL && testVR && testVLL && testVLR && !testVRL && !testVRR)
		{
			es[i]->edgePt = 0.5*(v1 + v2) + 0.25*vL - 0.125*(vLL + vLR);
			continue;
		}
		if (testVL && testVR && testVRL && testVRR && !testVLL && !testVLR)
		{
			es[i]->edgePt = 0.5*(v1 + v2) + 0.25*vR - 0.125*(vRL + vRR);
			continue;
		}
		if (testVL && testVR)
		{
			es[i]->edgePt = 0.5*(v1 + v2);
			continue;
		}
		es[i]->edgePt = 0.5*(v1 + v2);
		/*if (testVR && testVRL && testVRR)
		{
			vtemp = getOtherVertexfromTriangle(es[i], es[i]->t2);
			vR = vtemp->coord;
			etemp = getEdgefromVertexes(es, es[i]->v1, vtemp);
			vtemp2 = getOtherVertexfromTriangle(etemp, etemp->t1 != es[i]->t1 ? etemp->t1 : etemp->t2);
			vRL = vtemp2->coord;
			etemp = getEdgefromVertexes(es, es[i]->v1, vtemp2);
			vtemp2 = getOtherVertexfromTriangle(etemp, etemp->t1 != es[i]->t1 ? etemp->t1 : etemp->t2);


			etemp = getEdgefromVertexes(es, es[i]->v2, vtemp);
			vtemp2 = getOtherVertexfromTriangle(etemp, etemp->t1 != es[i]->t1 ? etemp->t1 : etemp->t2);
			if (vtemp2 != nullptr)
			{
				vRR = vtemp2->coord;
				testVRR = true;
			}
		}*/

	}

	int oldTriangleSize = ts.size();
	int oldEdgeSize = es.size();
	// Calculer les nouveaux triangles
	for (int i = 0; i < oldTriangleSize; i++)
	{
		Vertex *eP0 = new Vertex(ts[i]->tEs[0]->edgePt);
		Vertex *eP1 = new Vertex(ts[i]->tEs[1]->edgePt);
		Vertex *eP2 = new Vertex(ts[i]->tEs[2]->edgePt);
		vs.push_back(eP0);
		vs.push_back(eP1);
		vs.push_back(eP2);
		addTriangle(ts[i]->tVs[0], eP1, eP2, ts, es);
		addTriangle(ts[i]->tVs[1], eP0, eP2, ts, es);
		addTriangle(ts[i]->tVs[2], eP0, eP1, ts, es);
		addTriangle(eP0, eP1, eP2, ts, es);
	}
	// Supprimer les ancients edges
	for (int i = 0; i < oldEdgeSize; i++)
	{
		removeEdgefromVertex(es[i], es[i]->v1);
		removeEdgefromVertex(es[i], es[i]->v2);
		delete es[i];
	}
	es.erase(es.begin(), es.begin() + oldEdgeSize);

	// Supprimer les ancients triangles
	for (int i = 0; i < oldTriangleSize; i++)
		delete ts[i];
	ts.erase(ts.begin(), ts.begin() + oldTriangleSize);
	oldEdgeSize = 0;
}