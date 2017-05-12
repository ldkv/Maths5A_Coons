/********************************************************************************
** Form generated from reading UI file 'maths5a_coons.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATHS5A_COONS_H
#define UI_MATHS5A_COONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Maths5A_CoonsClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *centralLayout;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupMode;
    QGridLayout *gridLayout;
    QRadioButton *rbAdjust;
    QSpinBox *spinPrecision;
    QLabel *laHorizontal;
    QPushButton *bJoin;
    QLabel *label;
    QLabel *laVertical;
    QPushButton *bCancelJoin;
    QFrame *line;
    QSpinBox *spinHori;
    QSpinBox *spinJoinOrder;
    QPushButton *bResetData;
    QLabel *laPrecision;
    QSpinBox *spinVerti;
    QFrame *line_2;
    QLabel *laTimeCalcSurface;
    QRadioButton *rbRandom;
    QGroupBox *groupRotation;
    QGridLayout *gridLayout_3;
    QPushButton *buttonSubdivide;
    QPushButton *buttonGenerateCube;
    QRadioButton *rbButterfly;
    QRadioButton *rbKobbelt;
    QRadioButton *rbCatmull;
    QRadioButton *rbLoop;
    QCheckBox *cbShowWireframe;
    QCheckBox *cbShowTexture;
    QCheckBox *cbShowPoints;
    QCheckBox *cbShowGrid;
    QGroupBox *groupLight;
    QGridLayout *gridLayout_2;
    QCheckBox *cbDiffuse;
    QPushButton *bColorS1;
    QCheckBox *cbSpecular;
    QCheckBox *cbLight1;
    QPushButton *bColorS2;
    QCheckBox *cbLight2;
    QPushButton *bColorObj;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QCheckBox *cbLine;
    QCheckBox *cbLineChaikin;
    QLabel *labelAngle;
    QSpinBox *spinDegreeCurve;
    QPushButton *buttonCreateCurve;
    QPushButton *buttonGenerateCoons;
    QLabel *labelCoonsSmooth;
    QSpinBox *spinDegreeCoons;
    QFrame *line_3;
    QPushButton *buttonGenerateCoons4;
    QSpacerItem *verticalSpacer;
    QPushButton *bResetCam;
    QPushButton *bQuit;

    void setupUi(QMainWindow *Maths5A_CoonsClass)
    {
        if (Maths5A_CoonsClass->objectName().isEmpty())
            Maths5A_CoonsClass->setObjectName(QStringLiteral("Maths5A_CoonsClass"));
        Maths5A_CoonsClass->resize(1129, 973);
        centralWidget = new QWidget(Maths5A_CoonsClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        centralLayout = new QHBoxLayout();
        centralLayout->setSpacing(6);
        centralLayout->setObjectName(QStringLiteral("centralLayout"));

        horizontalLayout_2->addLayout(centralLayout);

        Maths5A_CoonsClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Maths5A_CoonsClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1129, 26));
        Maths5A_CoonsClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Maths5A_CoonsClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Maths5A_CoonsClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Maths5A_CoonsClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Maths5A_CoonsClass->setStatusBar(statusBar);
        dockWidget = new QDockWidget(Maths5A_CoonsClass);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidget = new QTabWidget(dockWidgetContents);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(300, 0));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupMode = new QGroupBox(tab);
        groupMode->setObjectName(QStringLiteral("groupMode"));
        gridLayout = new QGridLayout(groupMode);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        rbAdjust = new QRadioButton(groupMode);
        rbAdjust->setObjectName(QStringLiteral("rbAdjust"));
        rbAdjust->setChecked(true);

        gridLayout->addWidget(rbAdjust, 1, 1, 1, 1);

        spinPrecision = new QSpinBox(groupMode);
        spinPrecision->setObjectName(QStringLiteral("spinPrecision"));
        spinPrecision->setMinimum(2);
        spinPrecision->setMaximum(1000);
        spinPrecision->setValue(10);

        gridLayout->addWidget(spinPrecision, 6, 2, 1, 1);

        laHorizontal = new QLabel(groupMode);
        laHorizontal->setObjectName(QStringLiteral("laHorizontal"));

        gridLayout->addWidget(laHorizontal, 2, 1, 1, 1);

        bJoin = new QPushButton(groupMode);
        bJoin->setObjectName(QStringLiteral("bJoin"));

        gridLayout->addWidget(bJoin, 11, 2, 1, 1);

        label = new QLabel(groupMode);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 9, 1, 1, 1);

        laVertical = new QLabel(groupMode);
        laVertical->setObjectName(QStringLiteral("laVertical"));

        gridLayout->addWidget(laVertical, 4, 1, 1, 1);

        bCancelJoin = new QPushButton(groupMode);
        bCancelJoin->setObjectName(QStringLiteral("bCancelJoin"));

        gridLayout->addWidget(bCancelJoin, 11, 1, 1, 1);

        line = new QFrame(groupMode);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 8, 1, 1, 2);

        spinHori = new QSpinBox(groupMode);
        spinHori->setObjectName(QStringLiteral("spinHori"));
        spinHori->setMinimum(0);
        spinHori->setMaximum(20);
        spinHori->setValue(0);

        gridLayout->addWidget(spinHori, 2, 2, 1, 1);

        spinJoinOrder = new QSpinBox(groupMode);
        spinJoinOrder->setObjectName(QStringLiteral("spinJoinOrder"));
        spinJoinOrder->setMaximum(2);

        gridLayout->addWidget(spinJoinOrder, 9, 2, 1, 1);

        bResetData = new QPushButton(groupMode);
        bResetData->setObjectName(QStringLiteral("bResetData"));

        gridLayout->addWidget(bResetData, 15, 1, 1, 2);

        laPrecision = new QLabel(groupMode);
        laPrecision->setObjectName(QStringLiteral("laPrecision"));

        gridLayout->addWidget(laPrecision, 6, 1, 1, 1);

        spinVerti = new QSpinBox(groupMode);
        spinVerti->setObjectName(QStringLiteral("spinVerti"));
        spinVerti->setMinimum(0);
        spinVerti->setMaximum(20);
        spinVerti->setValue(0);

        gridLayout->addWidget(spinVerti, 4, 2, 1, 1);

        line_2 = new QFrame(groupMode);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 13, 1, 1, 2);

        laTimeCalcSurface = new QLabel(groupMode);
        laTimeCalcSurface->setObjectName(QStringLiteral("laTimeCalcSurface"));
        laTimeCalcSurface->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(laTimeCalcSurface, 14, 1, 1, 2);

        rbRandom = new QRadioButton(groupMode);
        rbRandom->setObjectName(QStringLiteral("rbRandom"));
        rbRandom->setChecked(false);

        gridLayout->addWidget(rbRandom, 0, 1, 1, 1);


        verticalLayout->addWidget(groupMode);

        groupRotation = new QGroupBox(tab);
        groupRotation->setObjectName(QStringLiteral("groupRotation"));
        gridLayout_3 = new QGridLayout(groupRotation);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        buttonSubdivide = new QPushButton(groupRotation);
        buttonSubdivide->setObjectName(QStringLiteral("buttonSubdivide"));

        gridLayout_3->addWidget(buttonSubdivide, 4, 1, 1, 1);

        buttonGenerateCube = new QPushButton(groupRotation);
        buttonGenerateCube->setObjectName(QStringLiteral("buttonGenerateCube"));

        gridLayout_3->addWidget(buttonGenerateCube, 3, 1, 1, 1);

        rbButterfly = new QRadioButton(groupRotation);
        rbButterfly->setObjectName(QStringLiteral("rbButterfly"));

        gridLayout_3->addWidget(rbButterfly, 4, 0, 1, 1);

        rbKobbelt = new QRadioButton(groupRotation);
        rbKobbelt->setObjectName(QStringLiteral("rbKobbelt"));

        gridLayout_3->addWidget(rbKobbelt, 1, 0, 1, 1);

        rbCatmull = new QRadioButton(groupRotation);
        rbCatmull->setObjectName(QStringLiteral("rbCatmull"));
        rbCatmull->setChecked(false);

        gridLayout_3->addWidget(rbCatmull, 1, 1, 1, 1);

        rbLoop = new QRadioButton(groupRotation);
        rbLoop->setObjectName(QStringLiteral("rbLoop"));
        rbLoop->setChecked(true);

        gridLayout_3->addWidget(rbLoop, 3, 0, 1, 1);


        verticalLayout->addWidget(groupRotation);

        cbShowWireframe = new QCheckBox(tab);
        cbShowWireframe->setObjectName(QStringLiteral("cbShowWireframe"));
        cbShowWireframe->setEnabled(true);
        cbShowWireframe->setChecked(true);

        verticalLayout->addWidget(cbShowWireframe);

        cbShowTexture = new QCheckBox(tab);
        cbShowTexture->setObjectName(QStringLiteral("cbShowTexture"));

        verticalLayout->addWidget(cbShowTexture);

        cbShowPoints = new QCheckBox(tab);
        cbShowPoints->setObjectName(QStringLiteral("cbShowPoints"));
        cbShowPoints->setChecked(true);

        verticalLayout->addWidget(cbShowPoints);

        cbShowGrid = new QCheckBox(tab);
        cbShowGrid->setObjectName(QStringLiteral("cbShowGrid"));

        verticalLayout->addWidget(cbShowGrid);

        groupLight = new QGroupBox(tab);
        groupLight->setObjectName(QStringLiteral("groupLight"));
        gridLayout_2 = new QGridLayout(groupLight);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        cbDiffuse = new QCheckBox(groupLight);
        cbDiffuse->setObjectName(QStringLiteral("cbDiffuse"));

        gridLayout_2->addWidget(cbDiffuse, 6, 0, 1, 1);

        bColorS1 = new QPushButton(groupLight);
        bColorS1->setObjectName(QStringLiteral("bColorS1"));

        gridLayout_2->addWidget(bColorS1, 1, 1, 1, 1);

        cbSpecular = new QCheckBox(groupLight);
        cbSpecular->setObjectName(QStringLiteral("cbSpecular"));

        gridLayout_2->addWidget(cbSpecular, 6, 1, 1, 1);

        cbLight1 = new QCheckBox(groupLight);
        cbLight1->setObjectName(QStringLiteral("cbLight1"));

        gridLayout_2->addWidget(cbLight1, 1, 0, 1, 1);

        bColorS2 = new QPushButton(groupLight);
        bColorS2->setObjectName(QStringLiteral("bColorS2"));
        bColorS2->setLayoutDirection(Qt::LeftToRight);

        gridLayout_2->addWidget(bColorS2, 3, 1, 1, 1);

        cbLight2 = new QCheckBox(groupLight);
        cbLight2->setObjectName(QStringLiteral("cbLight2"));

        gridLayout_2->addWidget(cbLight2, 3, 0, 1, 1);

        bColorObj = new QPushButton(groupLight);
        bColorObj->setObjectName(QStringLiteral("bColorObj"));

        gridLayout_2->addWidget(bColorObj, 0, 0, 1, 2);

        bColorS1->raise();
        bColorS2->raise();
        cbLight1->raise();
        cbLight2->raise();
        cbDiffuse->raise();
        cbSpecular->raise();
        bColorObj->raise();

        verticalLayout->addWidget(groupLight);


        verticalLayout_3->addLayout(verticalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        cbLine = new QCheckBox(groupBox);
        cbLine->setObjectName(QStringLiteral("cbLine"));
        cbLine->setEnabled(true);
        cbLine->setGeometry(QRect(10, 30, 91, 17));
        cbLine->setChecked(false);
        cbLineChaikin = new QCheckBox(groupBox);
        cbLineChaikin->setObjectName(QStringLiteral("cbLineChaikin"));
        cbLineChaikin->setGeometry(QRect(120, 30, 141, 17));
        labelAngle = new QLabel(groupBox);
        labelAngle->setObjectName(QStringLiteral("labelAngle"));
        labelAngle->setGeometry(QRect(10, 60, 125, 20));
        spinDegreeCurve = new QSpinBox(groupBox);
        spinDegreeCurve->setObjectName(QStringLiteral("spinDegreeCurve"));
        spinDegreeCurve->setGeometry(QRect(120, 60, 141, 20));
        spinDegreeCurve->setMinimum(1);
        spinDegreeCurve->setMaximum(1000);
        spinDegreeCurve->setValue(3);
        buttonCreateCurve = new QPushButton(groupBox);
        buttonCreateCurve->setObjectName(QStringLiteral("buttonCreateCurve"));
        buttonCreateCurve->setGeometry(QRect(10, 100, 251, 23));
        buttonGenerateCoons = new QPushButton(groupBox);
        buttonGenerateCoons->setObjectName(QStringLiteral("buttonGenerateCoons"));
        buttonGenerateCoons->setGeometry(QRect(10, 180, 251, 23));
        labelCoonsSmooth = new QLabel(groupBox);
        labelCoonsSmooth->setObjectName(QStringLiteral("labelCoonsSmooth"));
        labelCoonsSmooth->setGeometry(QRect(10, 150, 125, 20));
        spinDegreeCoons = new QSpinBox(groupBox);
        spinDegreeCoons->setObjectName(QStringLiteral("spinDegreeCoons"));
        spinDegreeCoons->setGeometry(QRect(120, 150, 141, 20));
        spinDegreeCoons->setMinimum(1);
        spinDegreeCoons->setMaximum(1000);
        spinDegreeCoons->setValue(3);
        line_3 = new QFrame(groupBox);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(10, 130, 251, 16));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        buttonGenerateCoons4 = new QPushButton(groupBox);
        buttonGenerateCoons4->setObjectName(QStringLiteral("buttonGenerateCoons4"));
        buttonGenerateCoons4->setGeometry(QRect(10, 210, 251, 23));

        verticalLayout_4->addWidget(groupBox);


        verticalLayout_5->addLayout(verticalLayout_4);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        bResetCam = new QPushButton(dockWidgetContents);
        bResetCam->setObjectName(QStringLiteral("bResetCam"));
        bResetCam->setCheckable(false);

        verticalLayout_2->addWidget(bResetCam);

        bQuit = new QPushButton(dockWidgetContents);
        bQuit->setObjectName(QStringLiteral("bQuit"));

        verticalLayout_2->addWidget(bQuit);

        dockWidget->setWidget(dockWidgetContents);
        Maths5A_CoonsClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        retranslateUi(Maths5A_CoonsClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Maths5A_CoonsClass);
    } // setupUi

    void retranslateUi(QMainWindow *Maths5A_CoonsClass)
    {
        Maths5A_CoonsClass->setWindowTitle(QApplication::translate("Maths5A_CoonsClass", "Math5A ESGI - Coons", 0));
        dockWidget->setWindowTitle(QApplication::translate("Maths5A_CoonsClass", "Configuration", 0));
        groupMode->setTitle(QApplication::translate("Maths5A_CoonsClass", "Mode de g\303\251n\303\251ration B\303\251ziers", 0));
        rbAdjust->setText(QApplication::translate("Maths5A_CoonsClass", "R\303\251glage de l'hauteur", 0));
        laHorizontal->setText(QApplication::translate("Maths5A_CoonsClass", "Degr\303\251 horizontal", 0));
        bJoin->setText(QApplication::translate("Maths5A_CoonsClass", "Raccorder", 0));
        label->setText(QApplication::translate("Maths5A_CoonsClass", "Ordre de raccordement", 0));
        laVertical->setText(QApplication::translate("Maths5A_CoonsClass", "Degr\303\251 vertical", 0));
        bCancelJoin->setText(QApplication::translate("Maths5A_CoonsClass", "Annuler", 0));
        bResetData->setText(QApplication::translate("Maths5A_CoonsClass", "R\303\251initialiser les donn\303\251es", 0));
        laPrecision->setText(QApplication::translate("Maths5A_CoonsClass", "Nombre de pas (pr\303\251cision)", 0));
        laTimeCalcSurface->setText(QApplication::translate("Maths5A_CoonsClass", "0 us", 0));
        rbRandom->setText(QApplication::translate("Maths5A_CoonsClass", "Al\303\251atoire", 0));
        groupRotation->setTitle(QApplication::translate("Maths5A_CoonsClass", "Subdivision", 0));
        buttonSubdivide->setText(QApplication::translate("Maths5A_CoonsClass", "Subdiviser", 0));
        buttonGenerateCube->setText(QApplication::translate("Maths5A_CoonsClass", "G\303\251n\303\251rer Cube", 0));
        rbButterfly->setText(QApplication::translate("Maths5A_CoonsClass", "Butterfly", 0));
        rbKobbelt->setText(QApplication::translate("Maths5A_CoonsClass", "Kobbelt", 0));
        rbCatmull->setText(QApplication::translate("Maths5A_CoonsClass", "Catmull-Clark", 0));
        rbLoop->setText(QApplication::translate("Maths5A_CoonsClass", "Loop", 0));
        cbShowWireframe->setText(QApplication::translate("Maths5A_CoonsClass", "Filaire", 0));
        cbShowTexture->setText(QApplication::translate("Maths5A_CoonsClass", "Texture", 0));
        cbShowPoints->setText(QApplication::translate("Maths5A_CoonsClass", "Afficher les points", 0));
        cbShowGrid->setText(QApplication::translate("Maths5A_CoonsClass", "Grille et les axes", 0));
        groupLight->setTitle(QApplication::translate("Maths5A_CoonsClass", "Lumi\303\250res", 0));
        cbDiffuse->setText(QApplication::translate("Maths5A_CoonsClass", "Diffuse", 0));
        bColorS1->setText(QString());
        cbSpecular->setText(QApplication::translate("Maths5A_CoonsClass", "Specular", 0));
        cbLight1->setText(QApplication::translate("Maths5A_CoonsClass", "Source 1", 0));
        bColorS2->setText(QString());
        cbLight2->setText(QApplication::translate("Maths5A_CoonsClass", "Source 2", 0));
        bColorObj->setText(QApplication::translate("Maths5A_CoonsClass", "Couleur du patch", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Maths5A_CoonsClass", "Subdivision", 0));
        groupBox->setTitle(QApplication::translate("Maths5A_CoonsClass", "Corner Cutting", 0));
        cbLine->setText(QApplication::translate("Maths5A_CoonsClass", "Afficher lignes", 0));
        cbLineChaikin->setText(QApplication::translate("Maths5A_CoonsClass", "Afficher lignes (Chainkin)", 0));
        labelAngle->setText(QApplication::translate("Maths5A_CoonsClass", "Degr\303\251 lissage", 0));
        buttonCreateCurve->setText(QApplication::translate("Maths5A_CoonsClass", "Valider Courbe", 0));
        buttonGenerateCoons->setText(QApplication::translate("Maths5A_CoonsClass", "G\303\251n\303\251rer Coons", 0));
        labelCoonsSmooth->setText(QApplication::translate("Maths5A_CoonsClass", "Degr\303\251 lissage", 0));
        buttonGenerateCoons4->setText(QApplication::translate("Maths5A_CoonsClass", "G\303\251n\303\251rer Coons (4)", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Maths5A_CoonsClass", "Coons", 0));
        bResetCam->setText(QApplication::translate("Maths5A_CoonsClass", "R\303\251initialiser le cam\303\251ra", 0));
        bQuit->setText(QApplication::translate("Maths5A_CoonsClass", "Quitter", 0));
    } // retranslateUi

};

namespace Ui {
    class Maths5A_CoonsClass: public Ui_Maths5A_CoonsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATHS5A_COONS_H
