#include "stdafx.h"
#include "GLWidget.h"
#include <GL/glu.h>

// Initialisation de la sc�ne OpenGL
GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), m_theta(180.0f), m_phi(0.0f), m_aspectRatio(1.0)
{
	int seconde = 1000; // 1 seconde = 1000 ms
	int timerInterval = seconde / 60;
	t_Timer = new QTimer(this);
	connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
	t_Timer->start(timerInterval);
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);

	// Scale de la sc�ne ((zoom)
	m_scale = 1;
	m_incrementScale = 1;
	// Position des deux lumi�res et couleurs ambiant et diffuse
	lights[0].posLight = { 0, 0, 300 };
	lights[0].iAmbiant = { 1.0,1.0,1.0 };
	lights[0].iDiffuse = { 1.0,1.0,1.0 };
	lights[1].posLight = { -100, 150, 150 };
	lights[1].iAmbiant = { 1.0,0.0,1.0 };
	lights[1].iDiffuse = { 1.0,0.0,1.0 };
}

// A la suppression du programme
GLWidget::~GLWidget()
{
	delete[] t_Timer;
}

// Pour garder les FPS � 60
void GLWidget::timeOutSlot()
{
	update();
}


vector<Point> cubePoints;
vector<Face> cubeFaces;
vector<int> cubeIndices;
vector<Face> dividedCube;

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

	createCube();
	dividedCube = subdivide(cubeFaces);
	//dividedCube = subdivide(dividedCube);
	//dividedCube = subdivide(dividedCube);
}

// Redimensionner de la sc�ne pour adapter � la fen�tre principale
void GLWidget::resizeGL(int width, int height)
{
	screenH = height;
	screenW = width;

	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// D�finition de la matrice orthogonale de la camera
	m_aspectRatio = double(width) / double(height);
	if (width <= height)
		glOrtho(-range, range, -range / m_aspectRatio, range / m_aspectRatio, range*4, -range*4);
	else
		glOrtho(-range * m_aspectRatio, range * m_aspectRatio, -range, range, range*4, -range*4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Fonction mettre � jour de la sc�ne OpenGL
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

// Fonction rendu de la sc�ne
void GLWidget::drawScene()
{
	// Afficher la grille et les Axes dans la sc�ne
	if (showGrid)
		drawGridandAxes();
	
	// On passe la posiiton des lights
	vector<QVector3D> ptlight = { lights[0].posLight };
	vector<QVector3D> ptlight2 = { lights[1].posLight };
	// Dessin de la repr�sentation des lights (points)
	drawPoints(ptlight, lights[0].iAmbiant, 20);
	drawPoints(ptlight2, lights[1].iAmbiant, 20);

	// Affichage des points de controle et des vertex du patch
	if (showPts)
	{
		// Surbriller les points de raccordement
		drawPoints(points, { 0, 1.0, 0 }, 10);
	}

	drawFaces(cubeFaces);
	drawFaces(dividedCube);
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

// Dessiner d'un r�seau des points
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

// Conversion de coordonn�es d'�cran � coordonn�es de la sc�ne OPENGL
QVector3D GLWidget::convertXY(int X, int Y)
{
	double rangeS = range / m_scale;
	return QVector3D((int)((double)X * 2.0 * rangeS * m_aspectRatio / screenW - rangeS * m_aspectRatio), (int)((double)Y * 2.0 * rangeS / screenH - rangeS), 0);
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
			points.push_back(QVector3D(convertXY(event->pos().x(), event->pos().y())));
			needUpdate = true;
		}
	}
	// Suppression d'un point
	else if (event->buttons() & Qt::RightButton)
	{
		if (pointSelected != -1)
		{
			points.erase(points.begin() + pointSelected);
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
			points[pointSelected] = convertXY(event->pos().x(), event->pos().y());
			needUpdate = true;
		}
	}
}

// Callback pour la rel�che de la souris
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
		QVector3D d = convertXY(p.x(), p.y()) - points[i];
		if (sqrt(pow(d.x(), 2) + pow(d.y(), 2) <= POINT_SIZE * 4))
			return i;
	}
	return -1;
}

// Dessiner la grille et les axes dans la sc�ne
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

// Les contr�les de clavier
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
	default:
		break;
	}
}

// Contr�le molette pour zoomer
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

// R�initialiser le cam�ra au param�tres par d�faut
void GLWidget::resetCamera() 
{
	m_theta = 180.0f;
	m_phi = 0.0f;
	m_scale = 1;
	//QApplication::setOverrideCursor(Qt::PointingHandCursor);
}

// R�initialiser les donn�es
void GLWidget::resetData()
{
	points.clear();
	degU = 0;
	degV = 0;
}

void GLWidget::createCube() {
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
	tmp = { QVector3D(-50, 50, -50), QVector3D(-50, 50, 50), QVector3D(50, 50, 50), QVector3D(50, 50, -50) };
	cubeFaces.push_back(tmp);
	tmp = { QVector3D(50, 50, -50), QVector3D(50, 50, 50), QVector3D(50, -50, 50), QVector3D(50, -50, -50) };
	cubeFaces.push_back(tmp);
}

void GLWidget::subcat() {

}