#pragma once

#include <vector>
#include <algorithm>
#include <random>

using namespace std;

#define PI 3.14159265358979

// Fonction interm�diaire pour l'algo de Casteljau
QVector3D getCasteljauPoint(vector<QVector3D> poly, double t);
// Calculer la surface de B�zier � partir d'un ensemble de points de contr�le
vector<vector<QVector3D>> calcSurfaceBezier(vector<vector<QVector3D>> pts, int precision);
// G�n�ration al�atoire d'un nombre entier dans l'intervalle donn�e
int randomGeneration(int min, int max);
// Convertir la classe QVector3D pour utiliser avec les fonctions OpenGL
void glVector3D(QVector3D p, bool vertex);
// Convertir les couleurs entre valeurs enti�res et normalis�es
QColor convertColor(QVector3D col);