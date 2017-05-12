#include "stdafx.h"
#include "GLWidget.h"
#include <GL/glu.h>

// Initialisation de la scène OpenGL
GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), m_theta(180.0f), m_phi(0.0f), m_aspectRatio(1.0)
{
	int seconde = 1000; // 1 seconde = 1000 ms
	int timerInterval = seconde / 60;
	t_Timer = new QTimer(this);
	connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
	t_Timer->start(timerInterval);
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);

	// Scale de la scène ((zoom)
	m_scale = 1;
	m_incrementScale = 1;
	// Position des deux lumières et couleurs ambiant et diffuse
	lights[0].posLight = { 0, 0, 300 };
	lights[0].iAmbiant = { 1.0,1.0,1.0 };
	lights[0].iDiffuse = { 1.0,1.0,1.0 };
	lights[1].posLight = { -100, 150, 150 };
	lights[1].iAmbiant = { 1.0,0.0,1.0 };
	lights[1].iDiffuse = { 1.0,0.0,1.0 };

	pointsChaikin.push_back(vector<QVector3D>());
	generateControlPointsTriangle();
}

// A la suppression du programme
GLWidget::~GLWidget()
{
	delete[] t_Timer;
}

// Pour garder les FPS à 60
void GLWidget::timeOutSlot()
{
	update();
}


vector<Point> cubePoints;
vector<Face> cubeFaces;
vector<int> cubeIndices;
vector<Face> dividedCube;
vector<Point> ptsControl;
vector<vector<Point>> ptsControlTriangle;
vector<Triangle*> tsBezier;
vector<Edge*> esBezier;
vector<Vertex*> vsBezier;
vector<Face> meshFaces;
vector<QVector3D> controlPoints;

// Initialisation du module OpenGL
void GLWidget::initializeGL()
{
	// Initialisation des proprietes OpenGL du programme
	glClearColor(0, 0, 0, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Angle de la camera
	range = 100.0;

	// Chargement de la texture
	LoadGLTextures("texture.jpg");

	generateControlPoints();
	//createCubeAlt();
	//dividedCube = subdivideC(cubeFaces);
	//dividedCube = subdivideC(dividedCube);
	//dividedCube = subdivideC(dividedCube);
	//meshFaces = subdivideC(meshFaces);
	//meshFaces = subdivideC(meshFaces);
}

// Redimensionner de la scène pour adapter à la fenêtre principale
void GLWidget::resizeGL(int width, int height)
{
	screenH = height;
	screenW = width;

	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Définition de la matrice orthogonale de la camera
	m_aspectRatio = double(width) / double(height);
	if (width <= height)
		glOrtho(-range, range, -range / m_aspectRatio, range / m_aspectRatio, range*4, -range*4);
	else
		glOrtho(-range * m_aspectRatio, range * m_aspectRatio, -range, range, range*4, -range*4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Fonction mettre à jour de la scène OpenGL
void GLWidget::paintGL()
{
	// Nettoyage des buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);

	// Shape Marking
	glPushMatrix();
	glRotatef(m_theta, 1.0f, 0.0f, 0.0f);
	glRotatef(m_phi, 0.0f, 0.0f, 1.0f);
	glScalef(m_scale, m_scale, m_scale);

	// Draw scene
	drawScene();

	glPopMatrix();
}

// Fonction rendu de la scène
void GLWidget::drawScene()
{
	// Afficher la grille et les Axes dans la scène
	if (showGrid)
		drawGridandAxes();
	
	// On passe la posiiton des lights
	vector<QVector3D> ptlight = { lights[0].posLight };
	vector<QVector3D> ptlight2 = { lights[1].posLight };
	// Dessin de la représentation des lights (points)
	drawPoints(ptlight, lights[0].iAmbiant, 20);
	drawPoints(ptlight2, lights[1].iAmbiant, 20);

	// Affichage des points de controle et des vertex du patch
	if (showPts)
	{
		// Surbriller les points de raccordement
		drawPoints(points, { 0, 1.0, 0 }, 10);
		//drawMesh(tsBezier, esBezier, WHITE, 20);
	}

	// Subdivision
	drawMesh(tsOri, esOri, BLUE, 5);
	drawMesh(ts, es, WHITE, 20);

	//drawFaces(cubeFaces);
	//drawFaces(dividedCube);
	//
	//drawFaces(meshFaces);
	if (showTexture) {
		//drawMesh(meshFaces);
	}
	/*drawFaces(meshFaces);
	if (showTexture) {
		drawMesh(meshFaces);
	}*/
	//drawPoints(controlPoints, { 0, 1.0, 0 }, 10);
}

void GLWidget::drawMesh(vector<Triangle*> ts, vector<Edge*> es, QVector3D color, int ptSize)
{
	for each (Triangle* t in ts)
	{
		/*glVector3D(color, false);
		glPointSize(ptSize);
		glBegin(GL_POINTS);
		for each (Vertex* v in t->tVs)
			glVector3D(v->coord, true);
		glEnd();*/
		
		glVector3D(color, false);
		glBegin(GL_LINE_LOOP);
			for each (Vertex* v in t->tVs)
				glVector3D(v->coord, true);
		glEnd();
	}

	if (showLine) {
		// Surbriller les lines
		drawLines(points, { 0, 1.0, 0 }, 4);
	}
	if (showChaikin)
	{
		drawChaikinLine({ 0.5f, 0.7f, 0.9f }, 4);
	}
}

// Generation des points de controle
void GLWidget::generateControlPoints()
{
	meshFaces.clear();

	// Si l'utilisateur n'a pas changé les valeurs minimales
	if (precision < 2 || degU < 2 || degV < 2)
		return;

	// Sinon, on efface les valeurs actuels des taleaux
	ptsControl.clear();
	//ptsJoin.clear();
	//ptsHighlighted.clear();

	//ptsControl.resize(degU);
	int x, y, z, zx, zy;

	// On fonction du style de genration de points que l'on souhaite obtenir

		x = -degU / 2 * 20;
		y = -degV / 2 * 20;
		zx = 0;
		zy = 0;
		// Pour chacune des valeurs de nos axes
		for (int i = 0; i < degU; i++)
		{
			for (int j = 0; j < degV; j++)
			{
				// On rajoute le point au tableau de points de controle
				ptsControl.push_back(Point(QVector3D(x, y, zy)));
				controlPoints.push_back(QVector3D(x, y, zy));
				y += 20;
				if (j < degV / 2)
					zy += 5 * depthBetweenPoints;
				else
					zy -= 5 * depthBetweenPoints;
			}
			y = -degV / 2 * 20;
			x += 20;
			if (i < degU / 2)
				zx += 5 * depthBetweenPoints;
			else
				zx -= 5 * depthBetweenPoints;
			zy = zx;
		}

		vector<Point> tmpPoints;
		int indexPass = degU - 1;
		for (size_t i = 0; i < (degU)*(degV)-degU; i++){
			if (i == indexPass) {
				indexPass+= degU;
			}
			else
			{
				tmpPoints.clear();
				tmpPoints.push_back(ptsControl[i]);
				tmpPoints.push_back(ptsControl[i + 1]);
				tmpPoints.push_back(ptsControl[i + degU + 1]);
				tmpPoints.push_back(ptsControl[i + degU]);
				meshFaces.push_back(tmpPoints);
			}
		}
		/*tmpPoints.clear();
		tmpPoints.push_back(ptsControl[(degU - 1)*(degV - 1) - 2]);
		tmpPoints.push_back(ptsControl[(degU - 1)*(degV - 1) - 2]);
		tmpPoints.push_back(ptsControl[0]);
		tmpPoints.push_back(ptsControl[0 + 1]);
		meshFaces.push_back(tmpPoints);*/
}

void GLWidget::generateControlPointsTriangle()
{
	// Si l'utilisateur n'a pas changé les valeurs minimales
	if (precision < 2 || degU < 2 || degV < 2)
		return;

	// Sinon, on efface les valeurs actuels des taleaux
	ptsControlTriangle.clear();

	ptsControlTriangle.resize(degU);
	int x, y, z, zx, zy;


	// Cas où l'on souhaite un placement régulier
	x = -degU / 2 * 20;
	y = -degV / 2 * 20;
	zx = 0;
	zy = 0;
	// Pour chacune des valeurs de nos axes
	for (int i = 0; i < degU; i++)
	{
		for (int j = 0; j < degV; j++)
		{
			// On rajoute le point au tableau de points de controle
			ptsControlTriangle[i].push_back(QVector3D(x, y, zy));
			y += 20;
			if (j < degV / 2)
				zy += 5 * depthBetweenPoints;
			else
				zy -= 5 * depthBetweenPoints;
		}
		y = -degV / 2 * 20;
		x += 20;
		if (i < degU / 2)
			zx += 5 * depthBetweenPoints;
		else
			zx -= 5 * depthBetweenPoints;
		zy = zx;
	}

	createBezierTriangle(tsBezier, esBezier, vsBezier, ptsControlTriangle);
}

Edge* updateEdgeCustom(vector<Edge*> &es, Vertex* p1, Vertex* p2, Triangle* t)
{
	int i = 0;
	for (i = 0; i < es.size(); i++)
	{
		if ((es[i]->v1->coord == p1->coord && es[i]->v2->coord == p2->coord)
			|| (es[i]->v1->coord == p2->coord && es[i]->v2->coord == p1->coord))
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

void addTriangleCustom(Vertex* p1, Vertex* p2, Vertex* p3, vector<Triangle*> &ts, vector<Edge*> &es)
{
	Triangle* t = new Triangle(p1, p2, p3);
	t->tEs.push_back(updateEdgeCustom(es, p2, p3, t));
	t->tEs.push_back(updateEdgeCustom(es, p3, p1, t));
	t->tEs.push_back(updateEdgeCustom(es, p1, p2, t));
	ts.push_back(t);
}

void GLWidget::createBezierTriangle(vector<Triangle*> &ts, vector<Edge*> &es, vector<Vertex*> &vs, vector<vector<Point>> points)
{
	for (int i = 0; i < points.size()-1; i++)
	{

		for (int j = 0; j < points[0].size()-1; j++)
		{
			Vertex* x0 = new Vertex(points[i][j].coord);
			vs.push_back(x0);
		}
	}
	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = 0; j < points[0].size() - 1; j++)
		{
			Vertex* x0 = new Vertex(points[i][j].coord);
			Vertex* x1 = new Vertex(points[i + 1][j + 1].coord);
			Vertex* x2 = new Vertex(points[i + 1][j].coord);
			Vertex* x3 = new Vertex(points[i][j + 1].coord);
			Vertex* x4 = new Vertex((points[i][j].coord.x()+ points[i + 1][j+1].coord.x())/2, (points[i][j].coord.y() + points[i + 1][j+1].coord.y()) / 2, (points[i][j].coord.z() + points[i + 1][j+1].coord.z()) / 2);
			
			addTriangleCustom(x0, x2, x4, ts, es);
			addTriangleCustom(x0, x4, x3, ts, es);
			addTriangleCustom(x4, x2, x1, ts, es);
			addTriangleCustom(x4, x1, x3, ts, es);
			vs.push_back(x4);
		}
	}
}

// Réinitialiser le caméra au paramètres par défaut
void GLWidget::subdivide(int choice)
{
	switch (choice)
	{
	case 1:
		Subdivision_Loop(ts, es, vs);
		break;
	case 2:
		Subdivision_Kobbelt(ts, es, vs);
		//Subdivision_Kobbelt(tsBezier, esBezier, vsBezier);
		break;
	case 3:
		Subdivision_Butterfly(ts, es, vs);
		break;
	default:
		break;
	}
}

void GLWidget::LoadGLTextures(const char * name)
{
	QImage img;

	if (!img.load(name)) {
		std::cerr << "ERROR in loading image" << std::endl;
	}

	QImage t = QGLWidget::convertToGLFormat(img);

	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLWidget::drawMesh(vector<Face> faces)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	for (int i = 0; i < faces.size(); i++)
	{
		//for (int j = 0; j <faces[i].points.size() - 3; j++)
		//{
			// Coordonnées des points avec coordonnées de la texture
			//glBegin(GL_QUADS);
			glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0f, 0.0f);  glVector3D(faces[i].points[0].coord, true);
			glTexCoord2f(1.0f, 1.0f);  glVector3D(faces[i].points[2].coord, true);
			glTexCoord2f(1.0f, 0.0f);  glVector3D(faces[i].points[1].coord, true);
			glTexCoord2f(0.0f, 0.0f);  glVector3D(faces[i].points[0].coord, true);
			glTexCoord2f(0.0f, 1.0f);  glVector3D(faces[i].points[3].coord, true);
			glTexCoord2f(1.0f, 1.0f);  glVector3D(faces[i].points[2].coord, true);
			glTexCoord2f(0.0f, 0.0f);  glVector3D(faces[i].points[0].coord, true);
			glTexCoord2f(1.0f, 1.0f);  glVector3D(faces[i].points[1].coord, true);
			glTexCoord2f(1.0f, 0.0f);  glVector3D(faces[i].points[2].coord, true);
			glTexCoord2f(0.0f, 0.0f);  glVector3D(faces[i].points[0].coord, true);
			glTexCoord2f(0.0f, 1.0f);  glVector3D(faces[i].points[2].coord, true);
			glTexCoord2f(1.0f, 1.0f);  glVector3D(faces[i].points[3].coord, true);
			/*glTexCoord2f(0.0f, 0.0f);  glVector3D(faces[i].points[j].coord, true);
			glTexCoord2f(0.0f, 1.0f);  glVector3D(faces[i].points[j+1].coord, true);
			glTexCoord2f(1.0f, 1.0f);  glVector3D(faces[i].points[j+2].coord, true);
			glTexCoord2f(1.0f, 0.0f);  glVector3D(faces[i].points[j+3].coord, true);
			glEnd();
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);  glVector3D(faces[i].points[j].coord, true);
			glTexCoord2f(1.0f, 0.0f);  glVector3D(faces[i].points[j + 3].coord, true);
			glTexCoord2f(1.0f, 1.0f);  glVector3D(faces[i].points[j + 2].coord, true);
			glTexCoord2f(0.0f, 1.0f);  glVector3D(faces[i].points[j + 1].coord, true);
			*/
			glEnd();
		//}
	}
	glDisable(GL_TEXTURE_2D);
}

QVector3D GLWidget::processLighting(QVector3D p1Face, QVector3D p2Face, QVector3D p3Face, QVector3D p4Face, Light light) 
{
	//glColor3fv(iAmbiant*kAmbiant);

	QVector3D u = p2Face - p1Face;
	QVector3D v = p4Face - p1Face;
	QVector3D normal = QVector3D::crossProduct(u, v).normalized();
	QVector3D dir = light.posLight - p1Face;
	float cosAngle = QVector3D::dotProduct(normal, dir) / (normal.length() * dir.length());
	cosAngle = (cosAngle <= 0) ? 0 : cosAngle;
	QVector3D R = dir - 2 * normal*(QVector3D::dotProduct(normal, dir));
	float ns = QVector3D::dotProduct(R, p1Face - posCam) / (R.length() * (p1Face - posCam).length());

	ns = (ns <= 0 || cosAngle <= 0) ? 0 : ns;
	ns = pow(ns, 32);

	float ior = 1.517f;
	QVector3D refraction;
	float k = 1.0 - ior * ior * (1.0 - QVector3D::dotProduct(normal, dir) * QVector3D::dotProduct(normal, dir));
	if (k < 0.0)
		R = { 0,0,0 };
	else
		R = ior * dir - (ior * QVector3D::dotProduct(normal, dir) + sqrt(k)) * normal;

	QVector3D l = light.iAmbiant * kAmbiant * objectColor;

	if (showLightDiffuse)
		l += light.iDiffuse * objectColor * kDiffuse * cosAngle;
	if (showLightSpecular)
		l += light.iDiffuse * kSpecular * ns;

	return l;
}

// Dessiner des points
void GLWidget::drawPoints(vector<QVector3D> pts, QVector3D color, int ptSize)
{
	int nbPoints = pts.size();
	if (nbPoints == 0)
		return;
	glVector3D(color, false);
	glPointSize(ptSize);
	glBegin(GL_POINTS);
	for (int i = 0; i < nbPoints; i++)
		glVector3D(pts[i], true);
	glEnd();
}

// Dessiner des lignes
void GLWidget::drawLines(vector<QVector3D> pts, QVector3D color, int lineWidth) {
	if (pts.size() == 0) return;
	glVector3D(color, false);
	glLineWidth(lineWidth);
	for (size_t j = 0; j < curveMaxPointIndice.size() + 1; j++)
	{
			vector<QVector3D> ptsBase = pts;
			if (curveMaxPointIndice.size() > 0) {
				if (j < curveMaxPointIndice.size())
					ptsBase.erase(ptsBase.begin() + curveMaxPointIndice[j] + 1, ptsBase.end());
				if (j != 0)
					ptsBase.erase(ptsBase.begin(), ptsBase.begin() + curveMaxPointIndice[j - 1] + 1);
			}

			if (ptsBase.size() > 1)
			{
				glBegin(GL_LINES);
				for (int i = 0; i < ptsBase.size() - 1; i++) {
					glVector3D(ptsBase[i], true);
					glVector3D(ptsBase[i + 1], true);
				}
				glEnd();
			}
	}
}

void GLWidget::drawChaikinLine(QVector3D color, int lineWidth) {
	int nbPoints = pointsChaikin.size();
	if (nbPoints <= 0) return;

	glVector3D(color, false);
	glLineWidth(lineWidth);
	for (size_t j = 0; j < pointsChaikin.size(); j++)
	{
		if (pointsChaikin[j].size() > 1) {
			glBegin(GL_LINES);
			for (int i = 0; i < pointsChaikin[j].size() - 1; i++) {
				glVector3D(pointsChaikin[j][i], true);
				glVector3D(pointsChaikin[j][i + 1], true);
			}
			glEnd();
		}
	}

	for (size_t i = 0; i < coonCurveIndice.size(); i+=2)
	{
		for (size_t j = coonCurveIndice[i]; j < coonCurveIndice[i+1]+1; j++)
		{
			glBegin(GL_LINES);
			for (size_t k = 0; k < pointsChaikin[j].size(); k++)
			{
				glVector3D(pointsChaikin[j][k], true);
				glVector3D(pointsChaikin[j + 1][k], true);
			}
			glEnd();
		}
	}

}

vector<QVector3D> GLWidget::getChaikinPoints(vector<QVector3D> pts, int degree) {
	vector<QVector3D> ptsChaikin = vector<QVector3D>();

	if (pts.size() >= 2) {
		vector<QVector3D> ptsBase = pts;
		for (size_t j = 0; j < degree; j++)
		{
			for (size_t i = 0; i < ptsBase.size() - 1; i++)
			{
				QVector3D vectorTmp = (ptsBase[i + 1] - ptsBase[i]) / 4.0;
				ptsChaikin.push_back(ptsBase[i] + vectorTmp);
				ptsChaikin.push_back(ptsBase[i] + 3.0 * vectorTmp);
			}

			if (j != degree - 1)
			{
				ptsBase = ptsChaikin;
				ptsChaikin.clear();
			}
		}
	}
	return ptsChaikin;
}

vector<vector<QVector3D>> GLWidget::getAllChaikinPoints(vector<QVector3D> pts, int degree) {
	vector<vector<QVector3D>> ptsChaikin = vector<vector<QVector3D>>();

	if (pts.size() >= 2 && degree > 0) {
		for (size_t k = 0; k < curveMaxPointIndice.size(); k++)
		{
			ptsChaikin.push_back(vector<QVector3D >());
			vector<QVector3D> ptsBase = pts;
			int start = k <= 0 ? 0 : curveMaxPointIndice[k-1];
			ptsBase.erase(pts.begin() + start, pts.begin() + curveMaxPointIndice[k]);
			ptsChaikin[k] = getChaikinPoints(ptsBase, degree);
		}
	}
	return ptsChaikin;
}

vector<vector<QVector3D>> GLWidget::generateCoonsSurface(vector<QVector3D> curve1, vector<QVector3D> curve2, int degree) {
	vector<vector<QVector3D>> pts = vector<vector<QVector3D>>();
	int size = curve1.size() < curve2.size() ? curve1.size() : curve2.size();

	if (size < 1) return pointsChaikin; // verification des inputs

	// réorganisation de l'ordre des courbes dans le vector pointsChaikin
	pts.push_back(curve1);
	pointsChaikin.pop_back();

	for (size_t i = 0; i < degree; i++)
	{
		vector<QVector3D> ptsTmp = vector<QVector3D>();
		float v = 1.0f / (1.0f + degree);
		for (size_t j = 0; j < size; j++)
		{
			float v2 = v*(i + 1);
			QVector3D vect = (1.0f - v2)*curve1[j] + v2 * curve2[j];
			ptsTmp.push_back(vect);
		}
		pts.push_back(ptsTmp);
	}
	pts.push_back(curve2);
	pointsChaikin.pop_back();

	return pts;
}

vector<vector<QVector3D>> GLWidget::generateCoonsSurface(vector<QVector3D> curve1, vector<QVector3D> curve2, vector<QVector3D> curve3, vector<QVector3D> curve4, int degree) {
	vector<vector<QVector3D>> pts = vector<vector<QVector3D>>();
	int size = curve1.size() < curve2.size() ? curve1.size() : curve2.size();
	int size2 = curve3.size() < curve4.size() ? curve3.size() : curve4.size();
	int sizeMax = size < size2 ? size : size2;

	if (sizeMax < 1) { // verification des inputs
		return pts;
	}

	// réorganisation de l'ordre des courbes dans le vector pointsChaikin
	pts.push_back(curve1);
	pointsChaikin.pop_back();

	for (size_t i = 0; i < degree; i++)
	{
		vector<QVector3D> ptsTmp = vector<QVector3D>();
		float vBase = 1.0f / (1.0f + degree);
		for (size_t j = 0; j < size; j++)
		{
			float v = vBase*(i + 1);
			float u = vBase*(j + 1);
			QVector3D vect =	(1.0f - v)* ((1.0f - u) * curve1[0] + u * curve1[curve1.size()-1] ) + 
								v * ((1.0f - u) * curve2[0] + u * curve2[curve2.size()-1]);
			ptsTmp.push_back(vect);
		}
		pts.push_back(ptsTmp);
	}
	pts.push_back(curve2);
	pointsChaikin.pop_back();

	return pts;
}

// Dessiner d'un réseau des points
void GLWidget::drawPointsMatrix(vector<vector<QVector3D>> pts, QVector3D color, int ptSize)
{
	if (pts.size() == 0 || pts[0].size() == 0)
		return;
	int m = pts.size();
	int n = pts[0].size();
	glVector3D(color, false);
	glPointSize(ptSize);
	glBegin(GL_POINTS);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			glVector3D(pts[i][j], true);
	glEnd();
}

void GLWidget::drawFaces(vector<Face> faces)
{
	int nbPoints = faces.size();
	if (nbPoints == 0)
		return;
	glColor3f(150.0f, 150.0f, 150.0f);
	for (int i = 0; i < faces.size(); i++)
	{
		for(int j = 0; j < faces[i].points.size()-1; j++) {
			glBegin(GL_LINES);
			glVertex3f(faces[i].points[j].coord.x(), faces[i].points[j].coord.y(), faces[i].points[j].coord.z());
			glVertex3f(faces[i].points[j+1].coord.x(), faces[i].points[j+1].coord.y(), faces[i].points[j+1].coord.z());
			glEnd();
		}
		glBegin(GL_LINES);
		glVertex3f(faces[i].points[faces[i].points.size()-1].coord.x(), faces[i].points[faces[i].points.size()-1].coord.y(), faces[i].points[faces[i].points.size()-1].coord.z());
		glVertex3f(faces[i].points[0].coord.x(), faces[i].points[0].coord.y(), faces[i].points[0].coord.z());
		glEnd();
	}
}

void GLWidget::processCoon4() {
	if (pointsChaikin.size() > 2) {
		coonsState++;
		int maxSize = pointsChaikin.size() - 1;
		if (coonsState == 1) {
			if (curveMaxPointIndice.size() == 2)
				points.push_back(points[0]);
			else
				points.push_back(points[curveMaxPointIndice[curveMaxPointIndice.size() - 3] + 1]); // first point of the second last curve
			computePoint();
		}
		else if (coonsState == 2) {
			points.push_back(points[curveMaxPointIndice[curveMaxPointIndice.size() - 2] + 1]); // first point of the last curve
			computePoint();
			vector<QVector3D> &tmp = pointsChaikin[pointsChaikin.size() - 1];
			tmp.insert(tmp.begin(), pointsChaikin[pointsChaikin.size() - 3][0]);
			pointsChaikin.back().push_back(pointsChaikin[pointsChaikin.size() - 2][0]);
			curveMaxPointIndice.push_back(points.size() - 1);
			pointsChaikin.push_back(vector<QVector3D>());
			points.push_back(points[curveMaxPointIndice[curveMaxPointIndice.size() - 3]]); // last point of the second last curve
			computePoint();
		}
		else if (coonsState == 3) {
			points.push_back(points[curveMaxPointIndice[curveMaxPointIndice.size() - 2]]); // last point of the last curve
			computePoint();
			vector<QVector3D> &tmp = pointsChaikin[pointsChaikin.size() - 1];
			tmp.insert(tmp.begin(), pointsChaikin[pointsChaikin.size() - 4].back());
			pointsChaikin.back().push_back(pointsChaikin[pointsChaikin.size() - 3].back());
			curveMaxPointIndice.push_back(points.size() - 1);
			pointsChaikin.push_back(vector<QVector3D>());

			
			vector<vector<QVector3D>> pointsTmp = generateCoonsSurface(pointsChaikin[pointsChaikin.size() - 5], pointsChaikin[pointsChaikin.size() - 4], pointsChaikin[pointsChaikin.size() - 3], pointsChaikin[pointsChaikin.size() - 2], coonsDegree);
			pointsChaikin.insert(pointsChaikin.end(), pointsTmp.begin(), pointsTmp.end());
			
			coonsState = 0;
		}

		qDebug() << "Generate Coons";
	}
}

// Conversion de coordonnées d'écran à coordonnées de la scène OPENGL
QVector3D GLWidget::convertXY(int X, int Y)
{
	double rangeS = range / m_scale;
	return QVector3D((int)((double)X * 2.0 * rangeS * m_aspectRatio / screenW - rangeS * m_aspectRatio), (int)((double)Y * 2.0 * rangeS / screenH - rangeS), 0);
}

QVector3D GLWidget::rotateXY(QVector3D tmp)
{
	QQuaternion quat1 = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), 180 - m_theta);
	QQuaternion quat2 = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), -m_phi);
	quat1.normalize();
	quat2.normalize();
	tmp = quat1.rotatedVector(tmp);
	tmp = quat2.rotatedVector(tmp);
	return tmp;
}

void GLWidget::computePoint() {
	vector<QVector3D> ptsBase = points;
	if (curveMaxPointIndice.size() > 0)
		ptsBase.erase(ptsBase.begin(), ptsBase.begin() + curveMaxPointIndice[curveMaxPointIndice.size() - 1] + 1);

	if (showChaikin) {
		pointsChaikin[pointsChaikin.size() - 1] = getChaikinPoints(ptsBase, chaikinDegree);
	}
}

// Callback pour les click de la souris
void GLWidget::mousePressEvent(QMouseEvent *event)
{
	pointSelected = findNearestPoint(event->pos());
	// Ajout d'un point
	if (event->buttons() & Qt::LeftButton)
	{
		if (pointSelected == -1)
		{
			QVector3D tmp = QVector3D(convertXY(event->pos().x(), event->pos().y()));
			tmp = rotateXY(tmp);
			points.push_back(tmp);
			
			computePoint();
			needUpdate = true;
		}
	}
	// Suppression d'un point
	else if (event->buttons() & Qt::RightButton)
	{
		if (pointSelected != -1)
		{
			points.erase(points.begin() + pointSelected);
			if (showChaikin)
				pointsChaikin = getAllChaikinPoints(points, chaikinDegree);
			needUpdate = true;
		}
	}
}

// Callback pour le mouvement de la souris
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	mousePos = event->pos();
	if (event->buttons() & Qt::LeftButton)
	{
		if (pointSelected >= 0)
		{
			QVector3D tmp = QVector3D(convertXY(event->pos().x(), event->pos().y()));
			tmp = rotateXY(tmp);
			points[pointSelected] = tmp;
			if (showChaikin)
				pointsChaikin = getAllChaikinPoints(points, chaikinDegree);
			needUpdate = true;
		}
	}
}

// Callback pour la relâche de la souris
void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && pointSelected >= 0)
	{
		//update();
	}
}

// Chercher du point (dans la nuage existante) la plus proche de la souris
int GLWidget::findNearestPoint(QPoint p)
{
	int nbPoints = points.size();
	for (int i = 0; i < nbPoints; i++)
	{
		QVector3D tmp = QVector3D(convertXY(p.x(), p.y()));
		tmp = rotateXY(tmp);
		tmp -= points[i];
		if (tmp.length() <= POINT_SIZE / 2)
			return i;
	}
	return -1;
}

// Dessiner la grille et les axes dans la scène
void GLWidget::drawGridandAxes()
{
	// Grid
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0.1, 0.1, 0.1);
	for (float x = -100; x < 100; x += 5)
	{
		glVertex3d(x, -100, 0);
		glVertex3d(x, 100, 0);
	}
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.1, 0.1, 0.1);
	for (float z = -100; z < 100; z += 5)
	{
		glVertex3d(-100, z, 0);
		glVertex3d(100, z, 0);
	}
	glEnd();

	// Axis
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 50, 0);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(50, 0, 0);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 50);
	glEnd();
}

// Les contrôles de clavier
void GLWidget::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key_Escape:
		qApp->quit();
		break;
	case Qt::Key_Left:
		m_phi -= 2.0f;
		break;
	case Qt::Key_Right:
		m_phi += 2.0f;
		break;
	case Qt::Key_Up:
		m_theta += 2.0f;
		break;
	case Qt::Key_Down:
		m_theta -= 2.0f;
		break;
	case Qt::Key_Return:
		validateCurve();
		break;
	case Qt::Key_M:
		generateCoons();
		break;
	case Qt::Key_C:
		processCoon4();
		break;
	case Qt::Key_Minus:
		depthBetweenPoints -= 1;
		generateControlPoints();
		break;
	case Qt::Key_Plus:
		depthBetweenPoints += 1;
		generateControlPoints();
		break;
	case Qt::Key_0:
		meshFaces = subdivideC(meshFaces);
		break;
	default:
		break;
	}
}

// Contrôle molette pour zoomer
void GLWidget::wheelEvent(QWheelEvent * event)
{	  
	m_incrementScale += event->delta() / 120;
	if (m_incrementScale == 0 && event->delta() / 120 > 0) 
	{
		m_incrementScale = 1.0f;
	}
	if (m_incrementScale == 0 && event->delta() / 120 < 0) 
	{
		m_incrementScale = -1.0f;
	}
	if (m_incrementScale < 0) 
	{
		m_scale = 1 / -m_incrementScale;
	}
	else
	{
		m_scale = m_incrementScale;
	}
	//resizeGL(screenH, screenW);
};

// Réinitialiser le caméra au paramètres par défaut
void GLWidget::resetCamera() 
{
	m_theta = 180.0f;
	m_phi = 0.0f;
	m_scale = 1;
	//QApplication::setOverrideCursor(Qt::PointingHandCursor);
}

// Réinitialiser les données
void GLWidget::resetData()
{
	points.clear();
	pointsChaikin.clear();
	pointsChaikin.push_back(vector<QVector3D>());
	curveMaxPointIndice.clear();
	coonCurveIndice.clear();
	coonsState = 0;
	degU = 0;
	degV = 0;

	for each (Triangle* t in ts)
		delete t;
	for each (Edge* e in es)
		delete e;
	for each (Vertex* v in vs)
		delete v;

	for each (Triangle* t in tsOri)
		delete t;
	for each (Edge* e in esOri)
		delete e;
	for each (Vertex* v in vsOri)
		delete v;
	ts.clear();
	es.clear();
	vs.clear();
	tsOri.clear();
	esOri.clear();
	vsOri.clear();
}

void GLWidget::createCubeAlt() {
	/*cubePoints.push_back(QVector3D(-50, -50, -50));
	cubePoints.push_back(QVector3D(-50, 50, -50));
	cubePoints.push_back(QVector3D(-50, -50, 50));
	cubePoints.push_back(QVector3D(-50, 50, 50));
	cubePoints.push_back(QVector3D(50, -50, -50));
	cubePoints.push_back(QVector3D(50, -50, 50));
	cubePoints.push_back(QVector3D(50, 50, 50));
	cubePoints.push_back(QVector3D(50, 50, -50));*/
	vector<Point> tmp = { QVector3D(-50, -50, -50), QVector3D(50, -50, -50), QVector3D(50, 50, -50), QVector3D(-50, 50, -50) };
	cubeFaces.push_back(tmp);
	tmp = { QVector3D(-50, -50, -50), QVector3D(-50, -50, 50), QVector3D(50, -50, 50), QVector3D(50, -50, -50) };
	cubeFaces.push_back(tmp);
	tmp = { QVector3D(-50, -50, -50), QVector3D(-50, 50, -50), QVector3D(-50, 50, 50), QVector3D(-50, -50, 50) };
	cubeFaces.push_back(tmp);
	tmp = { QVector3D(-50, 50, 50), QVector3D(50, 50, 50), QVector3D(50, -50, 50), QVector3D(-50, -50, 50) };
	cubeFaces.push_back(tmp);
	//tmp = { QVector3D(-50, 50, -50), QVector3D(-50, 50, 50), QVector3D(50, 50, 50), QVector3D(50, 50, -50) };
	tmp = { QVector3D(-50, 50, 50), QVector3D(50, 50, 50),  QVector3D(50, 200, 0) };
	cubeFaces.push_back(tmp);
	// Null
	tmp = { QVector3D(-50, 50, 50),  QVector3D(50, 200, 0), QVector3D(-50, 50, -50), };
	cubeFaces.push_back(tmp);
	// Null
	tmp = { QVector3D(-50, 50, -50), QVector3D(50, 200, 0), QVector3D(50, 50, -50) };
	cubeFaces.push_back(tmp);
	//tmp = { QVector3D(50, 50, -50), QVector3D(50, 50, 50), QVector3D(50, -50, 50), QVector3D(50, -50, -50) };
	tmp = { QVector3D(50, 50, -50), QVector3D(50, 200, 0), QVector3D(50, 50, 50), QVector3D(50, -50, 50),  QVector3D(50, -50, -50) };
	cubeFaces.push_back(tmp);
}

void GLWidget::subdivideCatmull()
{
	meshFaces = subdivideC(meshFaces);
}

void GLWidget::generateCube()
{
	resetData();
	createCube(ts, es, vs);
	createCube(tsOri, esOri, vsOri);
}

// Valider une courbe en cours de creation
void GLWidget::validateCurve()
{		// verification pour éviter d'incérer un doublon
	if (curveMaxPointIndice.size() == 0 || curveMaxPointIndice[curveMaxPointIndice.size() - 1] != points.size() - 1)
	{
		curveMaxPointIndice.push_back(points.size() - 1);
		pointsChaikin.push_back(vector<QVector3D>());

		qDebug() << "Validate Curve ";
	}
}

void GLWidget::generateCoons()
{
	// on a besoin d'au moins 2 courbes valides donc pointsChaikin.size() doit etre au minimum 3
	// car lorsqu'on press enter pour valider une courbe, on ajoute deja la prochaine courbe (vide), voir "case Qt::Key_Return"
	if (pointsChaikin.size() > 2)
	{
		int maxSize = pointsChaikin.size() - 1;
		vector<vector<QVector3D>> pointsTmp = generateCoonsSurface(pointsChaikin[maxSize - 2], pointsChaikin[maxSize - 1], coonsDegree);
		pointsChaikin.insert(pointsChaikin.end(), pointsTmp.begin(), pointsTmp.end());

		coonCurveIndice.push_back(maxSize - 2);
		coonCurveIndice.push_back(maxSize - 1 + coonsDegree);
		pointsChaikin.push_back(vector<QVector3D>());

		qDebug() << "Generate Coons";
	}
}

// Montrer/Cacher les ligne 
void GLWidget::displayLine(int val)
{
	if (val != 0)
		showLine = true;
	else
		showLine = false;
	qDebug() << "Show line : " << showLine;
}

// Montrer/Cacher les ligne (chainkin)
void GLWidget::displayLineChaikin(int val)
{
	if (val != 0)
		showChaikin = true;
	else
		showChaikin = false;

	qDebug() << "Show line : " << showChaikin;
}

void GLWidget::setChaikinDegree(int val)
{
	chaikinDegree = val;
	qDebug() << "Chainkin Degree : " << val;
}

void GLWidget::setCoonsDegree(int val)
{
	coonsDegree = val;
	qDebug() << "Coons Degree : " << val;
}