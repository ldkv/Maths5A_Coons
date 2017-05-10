
#include "stdafx.h"

#include "Subdivision_Catmull.h"
#include <QDebug>
#include <algorithm>  

void resetGlobalID()
{
	globalId = 0;
	globalSideId = 0;
	globalFaceId = 0;
}

Point getPointfromID(vector<Point> pts, int id)
{
	for (int i = 0; i < pts.size(); i++)
	{
		if (pts[i].id == id)
			return pts[i];
	}
	return Point();	// id = -1 => condition pour vérifier si on a bien trouvé id désiré
}

Side getSidefromID(vector<Side> sds, int id)
{
	for (int i = 0; i < sds.size(); i++)
	{
		if (sds[i].id == id)
			return sds[i];
	}
	return Side(); // id = -1 => condition pour vérifier si on a bien trouvé id désiré
}

Face getFacefromID(vector<Face> faces, int id)
{
	for (int i = 0; i < faces.size(); i++)
	{
		if (faces[i].id == id)
			return faces[i];
	}
	return Face();	// id = -1 => condition pour vérifier si on a bien trouvé id désiré
}

void deleteSidefromID(int id, vector<Side> &sds, vector<Point> &pts)
{
	for (int i = 0; i < sds.size(); i++)
	{
		if (sds[i].id == id)
		{
			deleteSidefromPoint(pts, sds[i].id, sds[i].pLow);
			deleteSidefromPoint(pts, sds[i].id, sds[i].pHigh);
			sds.erase(sds.begin() + i);
			break;
		}
	}
}

void deleteSidefromPoint(vector<Point> &pts, int sideID, int pointID)
{
	for (int i = 0; i < pts.size(); i++)
	{
		if (pts[i].id == pointID)
		{
			for (int j = 0; j < pts[i].sides.size(); j++)
			{
				if (pts[i].sides[j] == sideID)
				{
					pts[i].sides.erase(pts[i].sides.begin() + j);
					break;
				}
			}
			break;
		}
	}
}

void deleteFacefromSide(vector<Side> &sides, int sideID, int faceID)
{
	for (int i = 0; i < sides.size(); i++)
	{
		if (sides[i].id == sideID)
		{
			if (sides[i].fLeft == faceID)
				sides[i].fLeft = sides[i].fRight;
			sides[i].fRight = -1;
			break;
		}
	}
}


/*int addFace(int id1, int id2, int id3, vector<Face> &faces, vector<Side> &sides)
{
	faces.push_back(Face(id1, id2, id3));
	int newFaceID = faces[faces.size() - 1].id;
	for (int i = 0; i < sides.size(); i++)
	{
		if (sides[i].id == id1 || sides[i].id == id2 || sides[i].id == id3)
		{
			if (sides[i].fLeft == -1)
				sides[i].fLeft = newFaceID;
			else
				sides[i].fRight = newFaceID;
		}
	}
	return newFaceID;
}*/

void addSidetoPoint(vector<Point> &pts, int ptID, int sideID)
{
	for (int i = 0; i < pts.size(); i++)
		if (pts[i].id == ptID)
		{
			pts[i].sides.push_back(sideID);
			break;
		}
}

void removeSidefromPoint(vector<Point> &pts, int ptID, int sideID)
{
	for (int i = 0; i < pts.size(); i++)
		if (pts[i].id == ptID)
		{
			int k = 0;
			while (k++ < pts[i].sides.size() && pts[i].sides[k] != sideID);
			if (k < pts[i].sides.size())
				pts[i].sides.erase(pts[i].sides.begin() + k);
			break;
		}
}

// Chercher les arêtes incidentes depuis le point P
vector<Side> getIncidentEdgesOriented(Point P, vector<Side> sides, vector<Point> pts)
{
	vector<Side> edges;
	// Chercher une arête incidente externe depuis P
	for (int i = 0; i < P.sides.size(); i++)
	{
		if (getSidefromID(sides, P.sides[i]).fRight == -1)
		{
			// Echanger la place de cette arête au début
			int temp = P.sides[0];
			P.sides[0] = P.sides[i];
			P.sides[i] = temp;
			break;
		}
	}
	// La première arête sera choisie pour la référence de calcul des angles
	Side a = getSidefromID(sides, P.sides[0]);
	Point Q = getPointfromID(pts, a.pHigh == P.id ? a.pLow : a.pHigh);
	QLineF v(P.coord.x(), P.coord.y(), Q.coord.x(), Q.coord.y());
	vector<qreal> angles;
	angles.push_back(0);
	edges.push_back(a);

	for (int i = 1; i < P.sides.size(); i++)
	{
		a = getSidefromID(sides, P.sides[i]);
		Q = getPointfromID(pts, a.pHigh == P.id ? a.pLow : a.pHigh);
		QLineF PQ(P.coord.x(), P.coord.y(), Q.coord.x(), Q.coord.y());
		qreal angle = v.angleTo(PQ);
		int k = 0;
		for (k = 0; k < angles.size(); k++)
			if (angle <= angles[k])
				break;
		angles.insert(angles.begin() + k, angle);
		edges.insert(edges.begin() + k, a);
	}
	return edges;
}

int getSideIDfromPoints(vector<Side> sides, int p1, int p2)
{
	for (int i = 0; i < sides.size(); i++)
		if ((sides[i].pLow == p1 && sides[i].pHigh == p2) || (sides[i].pLow == p2 && sides[i].pHigh == p1))
			return sides[i].id;
	return -1;
}

int addSide(int p1, int p2, vector<Side> &sides, vector<Point> &pts)
{
	if (getSideIDfromPoints(sides, p1, p2) >= 0)
		return -1;
	sides.push_back(Side(p1, p2));
	int id = sides[sides.size() - 1].id;
	for (int i = 0; i < pts.size(); i++)
		if (pts[i].id == p1 || pts[i].id == p2)
			pts[i].sides.push_back(id);
	return id;
}

int getSideIDFromPoints(vector<Side> s, Point x, Point y) {
	for (int i = 0; i < s.size(); i++)
	{
		if ((s[i].pLow == x.id && s[i].pHigh == y.id) || (s[i].pHigh == x.id && s[i].pLow == y.id)) {
			return s[i].id;
		}
	}
	return -1;
}

int getPointIndex(vector<Point> pts, int id) {
	for (int i = 0; i < pts.size(); i++)
	{
		if (pts[i].id == id) {
			return i;
		}
	}
	return -1;
}

Point computeBarycenter(vector<Point> points)
{
	Point bary(QVector3D(0, 0, 0));
	int s = points.size();
	int sX = 0, sY = 0, sZ = 0;

	for (int i = 0; i < s; i++)
	{
		sX += points[i].coord.x();
		sY += points[i].coord.y();
		sZ += points[i].coord.z();
	}

	bary = Point(QVector3D(sX / s, sY / s, sZ / s));

	return bary;
}


vector<Face> getIncedentFaces(Face face, vector<Face> faces, int currentFace) {
	vector<Face> incidentFaces;
	int currentj = -1;
		for (size_t j = 0; j < faces.size(); j++)
		{
			for (size_t k = 0; k < faces[j].sides.size(); k++)
			{
				for (size_t i = 0; i < face.sides.size(); i++)
				{
					if (currentj != j && currentFace != j && face.sides[i] == faces[j].sides[k]) {
						incidentFaces.push_back(faces[j]);
						currentj = j;
					}
				}
			}

		}
		return incidentFaces;
}

Face getIncedentFace(Side side, vector<Face> faces) {
	Face incidentFace;
	for (size_t i = 0; i < faces.size(); i++)
	{
		for (size_t j = 0; j < faces[i].sides.size(); j++)
		{
				if (side == faces[i].sides[j]) {
					incidentFace = faces[i];
					break;
				}
		}

	}
	return incidentFace;
}

vector<Face> getFacesWithAPoint(Point p, vector<Face> faces) {
	vector<Face> incidentFaces;
	int currentj = -1;
	for (size_t j = 0; j < faces.size(); j++)
	{
		for (size_t k = 0; k < faces[j].points.size(); k++)
		{
			if (currentj != j &&  faces[j].points[k] == p) {
				incidentFaces.push_back(faces[j]);
				currentj = j;
			}
		}
	}
	return incidentFaces;
}

bool isInFaceVector(vector<Face> faces, Face face) {
	for (size_t i = 0; i < faces.size(); i++)
	{
		if (face == faces[i]) {
			return true;
		}
	}
	return false;
}

vector<Side> incidentEdgesToPoint(vector<Side> sides, Point point) {
	vector<Side> resultSides;
	for (size_t i = 0; i < sides.size(); i++)
	{
		if (sides[i].points[0].coord == point.coord || sides[i].points[1].coord == point.coord) {
			resultSides.push_back(sides[i]);
		}
	}
	return resultSides;
}

Point computeCenterSide(Point x, Point y) {
	return Point((x.coord+y.coord)/2);
}

Point computeVertexPoint(vector<Side> sides) {
	vector<Point> tmpPoints;
	for (size_t i = 0; i < sides.size(); i++)
	{
		tmpPoints.push_back(computeCenterSide(sides[i].points[0], sides[i].points[1]));
	}
	return computeBarycenter(tmpPoints);
}

vector<Face> subdivide(vector<Face> faces) {
	vector<Face> resultFaces;

	for (size_t i = 0; i < faces.size(); i++)
	{
		vector<Point> facePoints;
		vector<Point> edgePoints;
		Point currentFacePoint;
		vector<Point> vertexPoints;

		currentFacePoint = computeBarycenter(faces[i].points);
		vector<Face> incidentFaces;// getIncedentFaces(faces[i], faces, i);
		vector<Face> tmpFaceWithoutCurrent;
		for (size_t j = 0; j < faces.size(); j++)
		{
			if (i != j) tmpFaceWithoutCurrent.push_back(faces[j]);
		}
		for (size_t j = 0; j < faces[i].sides.size(); j++)
		{
			Face f = getIncedentFace(faces[i].sides[j], tmpFaceWithoutCurrent);
			if(f.points.size() != 0)
				incidentFaces.push_back(f);
		}
		for (size_t j = 0; j < incidentFaces.size(); j++)
		{
			//Point tmpBary = computeBarycenter(incidentFaces[j].points);
			facePoints.push_back(incidentFaces[j].barycenter);
			vector<Point> tmpPoints = { currentFacePoint , faces[i].sides[j].points[0], incidentFaces[j].barycenter, faces[i].sides[j].points[1] };
			edgePoints.push_back(computeBarycenter(tmpPoints));
		}

		for (size_t j = 0; j < faces[i].points.size(); j++)
		{
			vector<Face> incidentFacesPoint;
			vector<Face> FaceWithPointInCommon = getFacesWithAPoint(faces[i].points[j], faces);
			for (size_t k = 0; k < FaceWithPointInCommon.size(); k++)
			{
				if (!isInFaceVector(incidentFacesPoint, FaceWithPointInCommon[k])) {
					incidentFacesPoint.push_back(FaceWithPointInCommon[k]);
				}
			}
			Point Q;
			Point R;
			vector<Point> barycenterPoints;
			for (size_t k = 0; k < FaceWithPointInCommon.size(); k++)
			{
				barycenterPoints.push_back(FaceWithPointInCommon[k].barycenter);
			}
			Q = computeBarycenter(barycenterPoints);
			float n = static_cast<float>(incidentFacesPoint.size());

			vector<Side> tmpSides;
			for (size_t k = 0; k < FaceWithPointInCommon.size(); k++)
			{
				for (size_t h = 0; h < FaceWithPointInCommon[k].sides.size(); h++)
				{
					tmpSides.push_back(FaceWithPointInCommon[k].sides[h]);
				}
			}
			R = computeVertexPoint(incidentEdgesToPoint(tmpSides, faces[i].points[j]));

			vertexPoints.push_back(Point((Q.coord / n) + ((2 / n) * R.coord) + (((n - 3) / n) * faces[i].points[j].coord)));
		}
		
		for (size_t j = 0; j < faces[i].points.size() -1; j++)
		{
			vector<Point> tmpPoints;
			tmpPoints.push_back(currentFacePoint);
			tmpPoints.push_back(edgePoints[j]);
			tmpPoints.push_back(vertexPoints[j+1]);
			tmpPoints.push_back(edgePoints[j + 1]);
			resultFaces.push_back(Face(tmpPoints));
		}
		vector<Point> tmpPoints;
		tmpPoints.push_back(currentFacePoint);
		tmpPoints.push_back(edgePoints[faces[i].points.size() - 1]);
		tmpPoints.push_back(vertexPoints[0]);
		tmpPoints.push_back(edgePoints[0]);
		resultFaces.push_back(Face(tmpPoints));
	}

	return resultFaces;
}