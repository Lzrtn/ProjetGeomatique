#include <iostream>
#include <pqxx/pqxx>
#include <stdlib.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
#include <QWidget>
#include <QPointF>
#include <QCheckBox>
#include <QColor>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"
#include "dataflowwindow.h"

#include "2D/layer.h"
#include "2D/transformation.h"
#include "2D/shapefile.h"
#include "../outils/dbmanager.h"
#include "../outils/docker.h"

//Initialisation du Docker
// Creating container
std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
Docker docker(pathDockerFile);
// Get the Ip Adress
const std::string ipAdress_d = docker.getIpAdress();


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation in mode 2D //
    ui->stackedWidget->setCurrentIndex(mode);
    ui->label_attributeInformation->setVisible(!mode);
    ui->tableWidget_layerAttributeInformation->setVisible(!mode);
        // Action 2D
    ui->action_add2DVectorLayer->setEnabled(!mode);
    ui->action_add2DRastorLayer->setEnabled(!mode);
    ui->action_add2DDataFlow->setEnabled(!mode);
        // Actions 3D
    ui->action_add3DVectorLayer->setEnabled(mode);
    ui->action_add3DRastorLayer->setEnabled(mode);
    ui->action_add3DModel->setEnabled(mode);


    // Créer une scène pour QGraphicsView
    scene = new QGraphicsScene(this);

    // Définir la scène pour le QGraphicsView
    ui->graphicsView_window2D->setScene(scene);


    // Connecting switch 2D/3D button
    connect(ui->btn_switchMode2D3D, &QPushButton::clicked, this, &MainWindow::OnButtonSwitchTo2D3DClicked);

    // Connecting help action
    connect(ui->action_help, &QAction::triggered, this, &MainWindow::OnActionHelpClicked);

    // Connecting add2ddataflow action
    connect(ui->action_add2DDataFlow, &QAction::triggered, this, &MainWindow::OnAction2DDataFlowClicked);

    // Connecting add2dvectorlayer action and add3dvectorlayer action
    connect(ui->action_add2DVectorLayer, &QAction::triggered, this, &MainWindow::OnActionVectorLayerClicked);
    connect(ui->action_add3DVectorLayer, &QAction::triggered, this, &MainWindow::OnActionVectorLayerClicked);

    // Connecting add2drastorlayer action and add3drastorlayer action
    connect(ui->action_add2DRastorLayer, &QAction::triggered, this, &MainWindow::OnActionRastorLayerClicked);
    connect(ui->action_add3DRastorLayer, &QAction::triggered, this, &MainWindow::OnActionRastorLayerClicked);

    // Connecting add3dmodel action
    connect(ui->action_add3DModel, &QAction::triggered, this, &MainWindow::OnAction3DModelClicked);

    // Connecting ZoomIn button
    connect(ui->btn_zoomIn, &QPushButton::clicked, this, &MainWindow::OnButtonZoomIn);

    // Connecting ZoomOut button
    connect(ui->btn_zoomOut, &QPushButton::clicked, this, &MainWindow::OnButtonZoomOut);

    // Connecting ZoomOut button
    connect(ui->btn_zoomFull, &QPushButton::clicked, this, &MainWindow::OnButtonZoomFull);

    // Connection action "Add Vector File"
    //connect(ui->action_add2DVectorLayer, &QAction::triggered, this, &MainWindow::OnActionAddShpFileClicked);
    ipAdress = ipAdress_d;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;

    for(auto pair: layerList)
    {
        delete pair.second;
        layerList.erase(pair.first);
    }

    for (QGraphicsItem* item : ui->graphicsView_window2D->scene()->items())
    {
        delete item;
    }

}


Ui::MainWindow * MainWindow::getUi() const
{
    return ui;
}

void MainWindow::OnButtonSwitchTo2D3DClicked()
{
    mode = !mode;
    ui->stackedWidget->setCurrentIndex(mode);

    ui->stackedWidget->setCurrentIndex(mode);
    ui->tableWidget_layerAttributeInformation->setVisible(!mode);
    ui->label_attributeInformation->setVisible(!mode);

    ui->action_add2DVectorLayer->setEnabled(!mode);
    ui->action_add2DRastorLayer->setEnabled(!mode);
    ui->action_add2DDataFlow->setEnabled(!mode);

    ui->action_add3DVectorLayer->setEnabled(mode);
    ui->action_add3DRastorLayer->setEnabled(mode);
    ui->action_add3DModel->setEnabled(mode);

    this->update();

    ui->stackedWidget->update();
    ui->graphicsView_window2D->update();
    ui->openGLWidget_window3D->update();

    this->repaint();

    ui->stackedWidget->repaint();
    ui->graphicsView_window2D->repaint();
    ui->openGLWidget_window3D->repaint();
}

void MainWindow::OnActionHelpClicked()
{
    HelpWindow helpwindow;
    helpwindow.setModal(true);
    helpwindow.exec();
}

void MainWindow::OnAction2DDataFlowClicked()
{
    DataFlowWindow dataflowwindow;
    dataflowwindow.setModal(true);
    dataflowwindow.exec();
}

std::string MainWindow::OnActionVectorLayerClicked()
{
    QString fileNameVectorLayer = QFileDialog::getOpenFileName(this, tr("Ouvrir une couche de données vecteur"), "../../../", tr("ShapeFile (*.shp)"));
    std::string path = fileNameVectorLayer.toStdString();
    this->AddShpFileClicked(path);
    return path;
}

std::string MainWindow::OnActionRastorLayerClicked()
{
    QString fileNameRastorLayer = QFileDialog::getOpenFileName(this, tr("Ouvrir une couche de données raster"), "../../../", tr("GeoTIFF (*.tif)"));
    /*std::cout<<fileNameRastorLayer.toStdString()<<std::endl;*/
    return fileNameRastorLayer.toStdString();
}

std::string MainWindow::OnAction3DModelClicked()
{
    QString fileName3Dmodel = QFileDialog::getOpenFileName(this, tr("Ouvrir un modèle 3D"), "../../../", tr("Modèle 3D (*.gml *.asc)"));
    /*std::cout<<fileName3Dmodel.toStdString()<<std::endl;*/
    return fileName3Dmodel.toStdString();
}


void MainWindow::AddShpFileClicked(std::string path)
{
    // PostGreSQL Connection to the first database
    DbManager test("database2D", ipAdress);
    pqxx::connection conn(test.getString());

    if (conn.is_open()) {
        std::cout << "Connexion réussie à PostgreSQL" << std::endl;

    } else {
        std::cout << "Échec de la connexion à PostgreSQL" << std::endl;
        exit(1);
    }
    //import du shapefile dans la base de données
    Shapefile essai1 = Shapefile(path);


    essai1.import_to_db(test, 2154);

    //affichage des shapefiles importé
    test.Request("SELECT ST_AsGeoJSON(geom) FROM "+essai1.getTableName()+";");
    pqxx::result rowbis =test.getResult();
    QGraphicsItemGroup *layerGroup = essai1.plotShapefile(rowbis,scene);
    layerList[index] = new Layer("Layer "+QString::number(index), true, layerGroup);
    addLayerToListWidget(index, *layerList[index]);
    index++;


}

void MainWindow::OnButtonZoomIn()
{
    ui->graphicsView_window2D->scale(1.2,1.2);
    qreal currentScale = ui->graphicsView_window2D->transform().m11();

        // Parcourir tous les éléments de la scène
        for (QGraphicsItem* item : ui->graphicsView_window2D->scene()->items()) {
            QGraphicsPolygonItem* polyItem = dynamic_cast<QGraphicsPolygonItem*>(item);
            QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
            QGraphicsEllipseItem* pointItem = dynamic_cast<QGraphicsEllipseItem*>(item);

            if (polyItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = polyItem->pen();
                pen.setWidthF(adjustedWidth);
                polyItem->setPen(pen);
            }


            if (lineItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = lineItem->pen();
                pen.setWidthF(adjustedWidth);
                lineItem->setPen(pen);
            }

            if (pointItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = pointItem->pen();
                pen.setWidthF(adjustedWidth);
                pointItem->setPen(pen);
            }
        }
}

void MainWindow::OnButtonZoomOut()
{
    ui->graphicsView_window2D->scale(1/1.2,1/1.2);
    qreal currentScale = ui->graphicsView_window2D->transform().m11();

        // Parcourir tous les éléments de la scène
        for (QGraphicsItem* item : ui->graphicsView_window2D->scene()->items()) {
            QGraphicsPolygonItem* polyItem = dynamic_cast<QGraphicsPolygonItem*>(item);
            QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
            QGraphicsEllipseItem* pointItem = dynamic_cast<QGraphicsEllipseItem*>(item);

            if (polyItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = polyItem->pen();
                pen.setWidthF(adjustedWidth);
                polyItem->setPen(pen);
            }

            if (lineItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = lineItem->pen();
                pen.setWidthF(adjustedWidth);
                lineItem->setPen(pen);
            }

            if (pointItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = pointItem->pen();
                pen.setWidthF(adjustedWidth);
                pointItem->setPen(pen);
            }

        }
}

void MainWindow::OnButtonZoomFull()
{

    // Zoom sur l'ensemble des couches visibles
    QRectF visibleItemsRect;
    foreach (QGraphicsItem *item, scene->items()) {
        if (item->isVisible())
            visibleItemsRect = visibleItemsRect.united(item->sceneBoundingRect());
    }

    ui->graphicsView_window2D->fitInView(visibleItemsRect,Qt::KeepAspectRatio);



    qreal currentScale = ui->graphicsView_window2D->transform().m11();

        // Parcourir tous les éléments de la scène
        for (QGraphicsItem* item : ui->graphicsView_window2D->scene()->items()) {
            QGraphicsPolygonItem* polyItem = dynamic_cast<QGraphicsPolygonItem*>(item);
            QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
            QGraphicsEllipseItem* pointItem = dynamic_cast<QGraphicsEllipseItem*>(item);

            if (polyItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = polyItem->pen();
                pen.setWidthF(adjustedWidth);
                polyItem->setPen(pen);
            }


            if (lineItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = lineItem->pen();
                pen.setWidthF(adjustedWidth);
                lineItem->setPen(pen);
            }

            if (pointItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = pointItem->pen();
                pen.setWidthF(adjustedWidth);
                pointItem->setPen(pen);
            }
        }
}

void MainWindow::addLayerToListWidget(int index, Layer &layer) {

    // Créez un nouvel élément pour la couche
    layer.layerItem = new QListWidgetItem(ui->listeWidget_layersList);

    // Créez un widget personnalisé pour cet élément (contenant un label et une case à cocher)
    layer.layerWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(layer.layerWidget);

    QCheckBox *visibilityCheckbox = new QCheckBox("");
    visibilityCheckbox->setChecked(layer.isLayerVisible());
    QLabel *layerLabel = new QLabel(layer.getLayerName());


    // Connectez le signal clicked de la case à cocher à une fonction pour gérer la visibilité
    connect(visibilityCheckbox, &QCheckBox::toggled, [=](bool checked) {
        layerList[index]->getLayerGroup()->setVisible(checked);
        layerList[index]->setLayerVisible(checked);
    });


    layout->addWidget(visibilityCheckbox);
    layout->addWidget(layerLabel);
    layout->setAlignment(Qt::AlignLeft);

    layout->setContentsMargins(2, 0, 2, 0); // Taille de la ligne de la couche dans le gestionnaire
    layout->setSpacing(10); // Écart entre la checkbox et le nom de la couche


    layer.layerWidget->setLayout(layout);
    layer.layerItem->setSizeHint(layer.layerWidget->sizeHint());

    ui->listeWidget_layersList->setItemWidget(layer.layerItem, layer.layerWidget);

}
