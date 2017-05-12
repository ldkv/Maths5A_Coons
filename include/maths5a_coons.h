#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_maths5a_coons.h"
#include "GLWidget.h"

class Maths5A_Coons : public QMainWindow
{
	Q_OBJECT

public:
	Maths5A_Coons(QWidget *parent = Q_NULLPTR);

private slots:
	void setModeGenerationPoints();
	void resetData();
	void updateLabelTimer();
	void updateStatus();
	void setColor(int idButton);
	void quit();
	void generateCube();
	void subdivide();

private:
	Ui::Maths5A_CoonsClass ui;
	QButtonGroup *bGroup;
	GLWidget *glScene;
};
