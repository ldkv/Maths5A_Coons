
#include "stdafx.h"

#include "Subdivision_Catmull.h"
#include <QDebug>
#include <algorithm>  

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

bool isInPointVector(vector<Point> points, Point point) {
	for (size_t i = 0; i < points.size(); i++)
	{
		if (point == points[i]) {
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

// Fonction de subdivision
vector<Face> subdivide(vector<Face> faces, int sizex, int sizey) {
	// Résultat de la subdivision
	vector<Face> resultFaces;

	// Detection des points et des segments exterieurs (pour mesh ouvert)
	vector<Point> externPoints;
	vector<Side> externSides;
	for (size_t i = 0; i < faces.size(); i++)
	{
		// Toutes les faces sauf celle en cours
		vector<Face> tmpFaceWithoutCurrent;
		for (size_t j = 0; j < faces.size(); j++)
		{
			if (i != j) tmpFaceWithoutCurrent.push_back(faces[j]);
		}
		for (size_t j = 0; j < faces[i].sides.size(); j++)
		{
			// Si on a pas de face en commun avec un coté
			Face f = getIncedentFace(faces[i].sides[j], tmpFaceWithoutCurrent);
			if (f.points.size() == 0) {
				if(!isInPointVector(externPoints, faces[i].sides[j].points[0]))
				externPoints.push_back(faces[i].sides[j].points[0]);
				if (!isInPointVector(externPoints, faces[i].sides[j].points[1]))
				externPoints.push_back(faces[i].sides[j].points[1]);
				externSides.push_back(faces[i].sides[j]);
			}
		}
	}

	//int indexPass = sizex;
	// Pour chaque face
	for (size_t i = 0; i < faces.size(); i++)
	{
		/*if (i<sizex || i == indexPass || i == indexPass + sizex - 1 || i >= faces.size() - sizex) {
			if (i == indexPass + sizex - 1) {
				indexPass += sizex;
			}

		}*/

		vector<Point> facePoints;
		vector<Point> edgePoints;
		Point currentFacePoint;
		vector<Point> vertexPoints;

		// Barycentre de la face en cours
		currentFacePoint = computeBarycenter(faces[i].points);

		// Calcul des faces incidentes et edges sans incidences
		vector<Face> incidentFaces;// getIncedentFaces(faces[i], faces, i);
		vector<Face> tmpFaceWithoutCurrent;
		vector<int> indexEdgeWithNoFaceIncedent;
		for (size_t j = 0; j < faces.size(); j++)
		{
			if (i != j) tmpFaceWithoutCurrent.push_back(faces[j]);
		}
		for (size_t j = 0; j < faces[i].sides.size(); j++)
		{
			Face f = getIncedentFace(faces[i].sides[j], tmpFaceWithoutCurrent);
			if (f.points.size() != 0) {
				incidentFaces.push_back(f);
			}
			else
			{
				vector<Point> tmpPoints = { faces[i].sides[j].points[0], faces[i].sides[j].points[1], faces[i].sides[j].points[0], faces[i].sides[j].points[1] };
				incidentFaces.push_back(Face(tmpPoints));
				indexEdgeWithNoFaceIncedent.push_back(j);
			}
		}

		// Vertex points
		for (size_t j = 0; j < faces[i].points.size(); j++)
		{
			// Points qui ont une face incidente
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

			// Est ce que on est dans le cas d'un point mesh ouvert ?
			bool notSpecial = true;
			/*for (size_t k = 0; k < indexEdgeWithNoFaceIncedent.size(); k++)
			{
				if (faces[i].points[j] == faces[i].sides[k].points[0] || faces[i].points[j] == faces[i].sides[k].points[1]) {
					notSpecial = false;
				}
			}*/
			for (size_t k = 0; k < externPoints.size(); k++)
			{
				if (faces[i].points[j] == externPoints[k]) {
				notSpecial = false;
				}
			}
			if (notSpecial) {
				vertexPoints.push_back(Point((Q.coord / n) + ((2 / n) * R.coord) + (((n - 3) / n) * faces[i].points[j].coord)));
			}
			else
			{
				// Si mesh ouvert
				Point p1, p2, p3 = faces[i].points[j];
				bool p1b = false, p2b = false;
				for (size_t i = 0; i < externSides.size(); i++)
				{
					if (p3 == externSides[i].points[0] && p2 != externSides[i].points[1] && !p1b) {
						p1 = externSides[i].points[1];
						p1b = true;
					}
					if (p3 == externSides[i].points[1] && p2 != externSides[i].points[0] && !p1b) {
						p1 = externSides[i].points[0];
						p1b = true;
					}
					if (p3 == externSides[i].points[0] && p1 != externSides[i].points[1] && !p2b) {
						p2 = externSides[i].points[1];
						p2b = true;
					}
					if (p3 == externSides[i].points[1] && p1 != externSides[i].points[0] && !p2b) {
						p2 = externSides[i].points[0];
						p2b = true;
					}
				}
				vertexPoints.push_back(Point(6.0 / 8.0 * p3.coord + p1.coord/8 + p2.coord/8));
			}
		}

		// Edge Points
		for (size_t j = 0; j < incidentFaces.size(); j++)
		{
			//Point tmpBary = computeBarycenter(incidentFaces[j].points);
			facePoints.push_back(incidentFaces[j].barycenter);
			bool istrue = true;
			for (size_t k = 0; k < indexEdgeWithNoFaceIncedent.size(); k++)
			{
				if (j == indexEdgeWithNoFaceIncedent[k]) {
					istrue = false;
				}
			}
			if (istrue) {
				vector<Point> tmpPoints;
				tmpPoints = { currentFacePoint , faces[i].sides[j].points[0], incidentFaces[j].barycenter, faces[i].sides[j].points[1] };
				edgePoints.push_back(computeBarycenter(tmpPoints));
			}
			else
			{
				// Avec points perturbés
				bool notSpecial = true;
				for (size_t k = 0; k < externPoints.size(); k++)
				{
					if (faces[i].points[j] == externPoints[k]) {
						notSpecial = false;
					}
				}
				vector<Point> tmpPoints;
				if(j < incidentFaces.size() - 1) {
					tmpPoints = { vertexPoints[j],  vertexPoints[j + 1] };
				}
				else
				{
					tmpPoints = { vertexPoints[0],  vertexPoints[j] };
				}
				//edgePoints.push_back(computeBarycenter(tmpPoints));
				////////

				// Sans points perturbés
				edgePoints.push_back(incidentFaces[j].barycenter);
			}
		}
		
		// Création des faces
		for (size_t j = 0; j < incidentFaces.size() -1; j++)
		{
			vector<Point> tmpPoints;
			tmpPoints.push_back(currentFacePoint);
			tmpPoints.push_back(edgePoints[j]);
			tmpPoints.push_back(vertexPoints[j+1]);
			tmpPoints.push_back(edgePoints[j + 1]);
			resultFaces.push_back(Face(tmpPoints));
		}
		// Créattion de la dernière face
		vector<Point> tmpPoints;
		tmpPoints.push_back(currentFacePoint);
		tmpPoints.push_back(edgePoints[incidentFaces.size() - 1]);
		tmpPoints.push_back(vertexPoints[0]);
		tmpPoints.push_back(edgePoints[0]);
		resultFaces.push_back(Face(tmpPoints));
	}

	return resultFaces;
}