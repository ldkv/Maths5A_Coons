#include "stdafx.h"
#include "maths5a_coons.h"

Maths5A_Coons::Maths5A_Coons(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	glScene = new GLWidget(this);
	ui.centralLayout->addWidget(glScene);

	// Créer les raccourcis clavier
	// Ctrl + Q pour quitter
	QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
	QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(quit()));
	// Ctrl + R pour reset le caméra
	shortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
	QObject::connect(shortcut, SIGNAL(activated()), glScene, SLOT(resetCamera()));
	// Ctrl + D pour réinitialsier les données
	shortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
	QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(resetData()));

	// Connect signals aux éléments de l'UI
	// Interface génération des points
	connect(ui.rbRandom, SIGNAL(clicked()), this, SLOT(setModeGenerationPoints()));
	connect(ui.rbAdjust, SIGNAL(clicked()), this, SLOT(setModeGenerationPoints()));
	connect(ui.spinHori, SIGNAL(valueChanged(int)), glScene, SLOT(setDegreeU(int)));
	connect(ui.spinVerti, SIGNAL(valueChanged(int)), glScene, SLOT(setDegreeV(int)));
	connect(ui.spinPrecision, SIGNAL(valueChanged(int)), glScene, SLOT(setPrecision(int)));
	connect(ui.spinJoinOrder, SIGNAL(valueChanged(int)), glScene, SLOT(setJoinOrder(int)));
	connect(ui.bJoin, SIGNAL(clicked()), glScene, SLOT(generateJoinPatch()));
	connect(ui.bCancelJoin, SIGNAL(clicked()), glScene, SLOT(cancelJoin()));

	// Interface de Subdivision
	connect(ui.bSubdivide, SIGNAL(clicked()), glScene, SLOT(subdivide()));

	// Interface lumières + traçage
	connect(ui.cbShowWireframe, SIGNAL(stateChanged(int)), glScene, SLOT(setWireframe(int)));
	connect(ui.cbShowPoints, SIGNAL(stateChanged(int)), glScene, SLOT(setShowPts(int)));
	connect(ui.cbShowGrid, SIGNAL(stateChanged(int)), glScene, SLOT(setGrid(int)));
	connect(ui.cbShowTexture, SIGNAL(stateChanged(int)), glScene, SLOT(setShowTexture(int)));
	connect(ui.cbLight1, SIGNAL(stateChanged(int)), glScene, SLOT(setShowLight1(int)));
	connect(ui.cbLight2, SIGNAL(stateChanged(int)), glScene, SLOT(setShowLight2(int)));
	connect(ui.cbDiffuse, SIGNAL(stateChanged(int)), glScene, SLOT(setShowLightDiffuse(int)));
	connect(ui.cbSpecular, SIGNAL(stateChanged(int)), glScene, SLOT(setShowLightSpecular(int)));
	connect(ui.bResetData, SIGNAL(clicked()), this, SLOT(resetData()));
	connect(ui.bResetCam, SIGNAL(clicked()), glScene, SLOT(resetCamera()));
	connect(ui.bQuit, SIGNAL(clicked()), this, SLOT(quit()));
	//Chainkin
	connect(ui.cbLine, SIGNAL(stateChanged(int)), this, SLOT(showLine()));
	connect(ui.cbLineChainkin, SIGNAL(stateChanged(int)), this, SLOT(showLineChaikin()));
	connect(ui.spinAngle, SIGNAL(valueChanged(int)), this, SLOT(setAngle()));
	// Définir la couleur des boutons et leur signal
	QColor col = convertColor(glScene->objectColor);
	QString qss = QString("background-color: %1").arg(col.name());
	ui.bColorObj->setStyleSheet(qss);
	col = convertColor(glScene->lights[0].iAmbiant);
	qss = QString("background-color: %1").arg(col.name());
	ui.bColorS1->setStyleSheet(qss);
	col = convertColor(glScene->lights[1].iAmbiant);
	qss = QString("background-color: %1").arg(col.name());
	ui.bColorS2->setStyleSheet(qss);
	bGroup = new QButtonGroup(this);
	bGroup->addButton(ui.bColorS1, 0);
	bGroup->addButton(ui.bColorS2, 1);
	bGroup->addButton(ui.bColorObj, 2);
	connect(bGroup, SIGNAL(buttonClicked(int)), this, SLOT(setColor(int)));
	// Rotation Quaternion
	connect(ui.rbRotObj, SIGNAL(clicked()), this, SLOT(setModeRotation()));
	connect(ui.rbRotCam, SIGNAL(clicked()), this, SLOT(setModeRotation()));
	// Signal depuis le GLWidget
	connect(glScene, SIGNAL(labelChanged()), this, SLOT(updateLabelTimer()));
	connect(glScene, SIGNAL(mouseMoved()), this, SLOT(updateStatus()));

}

// Choisir la couleur et changer celle du bouton correspondant
void Maths5A_Coons::setColor(int id)
{
	QColor initial;
	if (id == 2)
		initial = convertColor(glScene->objectColor);
	else
		initial = convertColor(glScene->lights[id].iAmbiant);
	QColor col = QColorDialog::getColor(initial, this);
	if (col.isValid())
	{
		QString qss = QString("background-color: %1").arg(col.name());
		if (id == 2)
		{
			ui.bColorObj->setStyleSheet(qss);
			glScene->objectColor = QVector3D(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0);
		}
		else
		{
			if (id)
				ui.bColorS2->setStyleSheet(qss);
			else
				ui.bColorS1->setStyleSheet(qss);
			glScene->lights[id].iAmbiant = QVector3D(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0);;
			glScene->lights[id].iDiffuse = glScene->lights[id].iAmbiant;
		}
	}
}

// Mettre à jour le mode de génération des points de contrôle
void Maths5A_Coons::setModeGenerationPoints()
{
}

// Remise à défaut le mode de génération
void Maths5A_Coons::resetData()
{
	ui.spinHori->setValue(0);
	ui.spinVerti->setValue(0);
	glScene->resetData();
}

// Quitter
void Maths5A_Coons::updateLabelTimer()
{
	ui.laTimeCalcSurface->setText(glScene->labelTimer);
}

// Mettre à jour la position de la souris dans le status bar
void Maths5A_Coons::updateStatus()
{
	ui.statusBar->showMessage((QString("Screen (%1,%2) - World (%3,%4)")
		.arg(glScene->mouse.x())
		.arg(glScene->mouse.y())
		.arg(glScene->mouseWorld.x())
		.arg(glScene->mouseWorld.y())
		));
}

void Maths5A_Coons::showLine()
{
	qDebug() << ui.cbLine->isChecked();
}

void Maths5A_Coons::showLineChaikin()
{
	qDebug() << ui.cbLineChainkin->isChecked();
}

void Maths5A_Coons::setAngle()
{
	qDebug() << ui.spinAngle->value();
}

// Quitter
void Maths5A_Coons::quit()
{
	qApp->quit();
}