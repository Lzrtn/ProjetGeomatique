#include <iostream>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation in mode 2D //
    ui->stackedWidget->setCurrentIndex(mode);
    ui->btn_cameraRotation->setVisible(mode);
    ui->label_attributeInformation->setVisible(!mode);
    ui->tableWidget_layerAttributeInformation->setVisible(!mode);
        // Action 2D
    ui->action_add2DVectorLayer->setEnabled(!mode);
    ui->action_add2DRastorLayer->setEnabled(!mode);
    ui->action_add2DDataStream->setEnabled(!mode);
        // Actions 3D
    ui->action_add3DVectorLayer->setEnabled(mode);
    ui->action_add3DRastorLayer->setEnabled(mode);
    ui->action_add3DModel->setEnabled(mode);


    /*_______________________________TEST______________________________________*/


    // Créer une scène pour QGraphicsView
    QGraphicsScene *scene = new QGraphicsScene(this);

    // Définir la scène pour le QGraphicsView
    ui->graphicsView_window2D->setScene(scene);


    QVector<QPointF> franceCoordinates = {
        QPointF(-5.0, 42.0),
        QPointF(9.0, 42.0),
        QPointF(9.0, 51.0),
        QPointF(3.0, 51.0),
        QPointF(0.0, 48.0),
        QPointF(-50.0, 48.0)
        // Ajoutez d'autres points selon vos besoins
    };

    // Coordonnées du deuxième polygone à côté du premier
    QVector<QPointF> secondPolygonCoordinates = {
        QPointF(10.0, 42.0),
        QPointF(24.0, 42.0),
        QPointF(24.0, 51.0),
        QPointF(18.0, 51.0),
        QPointF(15.0, 48.0),
        QPointF(10.0, 48.0)
        // Ajoutez d'autres points selon vos besoins
    };


    // Créez un polygone à partir des coordonnées
    QPolygonF francePolygon(franceCoordinates);

    // Créez un polygone à partir des coordonnées du deuxième polygone
    QPolygonF secondPolygon(secondPolygonCoordinates);

    // Créez un objet QGraphicsPolygonItem
    QGraphicsPolygonItem *francePolygonItem = new QGraphicsPolygonItem(francePolygon);

    // Créez un objet QGraphicsPolygonItem pour le deuxième polygone
    QGraphicsPolygonItem *secondPolygonItem = new QGraphicsPolygonItem(secondPolygon);

    // Ajoutez l'élément du deuxième polygone à la scène
    scene->addItem(secondPolygonItem);

    // Ajoutez l'élément à la scène
    scene->addItem(francePolygonItem);




    /*_____________________________________________________________________________*/




    // Connecting switch 2D/3D button
    connect(ui->btn_switchMode2D3D, &QPushButton::clicked, this, &MainWindow::OnButtonSwitchTo2D3DClicked);

    // Connection action "Add Vector File"
    connect(ui->action_add2DVectorLayer, &QAction::triggered, this, &MainWindow::OnActionAddShpFileClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::OnButtonSwitchTo2D3DClicked()
{
    mode = !mode;
    ui->stackedWidget->setCurrentIndex(mode);
    ui->btn_cameraRotation->setVisible(mode);
    ui->tableWidget_layerAttributeInformation->setVisible(!mode);
    ui->label_attributeInformation->setVisible(!mode);

    ui->action_add2DVectorLayer->setEnabled(!mode);
    ui->action_add2DRastorLayer->setEnabled(!mode);
    ui->action_add2DDataStream->setEnabled(!mode);

    ui->action_add3DVectorLayer->setEnabled(mode);
    ui->action_add3DRastorLayer->setEnabled(mode);
    ui->action_add3DModel->setEnabled(mode);

    this->update();
    ui->btn_cameraRotation->update();
    ui->stackedWidget->update();
    ui->graphicsView_window2D->update();
    ui->openGLWidget_window3D->update();

    this->repaint();
    ui->btn_cameraRotation->repaint();
    ui->stackedWidget->repaint();
    ui->graphicsView_window2D->repaint();
    ui->openGLWidget_window3D->repaint();
}


void MainWindow::OnActionAddShpFileClicked()
{


//    std::cout<<"addVector"<<std::endl;
}


