#include <iostream>
#include <pqxx/pqxx>

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

#include "layer.h"
#include "transformation.h"
#include "shapefile.h"

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

    // Créer une scène pour QGraphicsView
    scene = new QGraphicsScene(this);

    // Définir la scène pour le QGraphicsView
    ui->graphicsView_window2D->setScene(scene);


    // Connecting switch 2D/3D button
    connect(ui->btn_switchMode2D3D, &QPushButton::clicked, this, &MainWindow::OnButtonSwitchTo2D3DClicked);

    // Connecting ZoomIn button
    connect(ui->btn_zoomIn, &QPushButton::clicked, this, &MainWindow::OnButtonZoomIn);

    // Connecting ZoomOut button
    connect(ui->btn_zoomOut, &QPushButton::clicked, this, &MainWindow::OnButtonZoomOut);

    // Connecting ZoomOut button
    connect(ui->btn_zoomFull, &QPushButton::clicked, this, &MainWindow::OnButtonZoomFull);

    // Connection action "Add Vector File"
    connect(ui->action_add2DVectorLayer, &QAction::triggered, this, &MainWindow::OnActionAddShpFileClicked);


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
    std::string path_fredo = "/home/fredo/Documents/projet_miniSIG/ProjetGeomatique/DONNEES_BDTOPO";
    std::string path_vicouille = "/home/formation/Documents/ProjetGeomatique/DONNEES_BDTOPO";

    //import d'un shapefile dans la base de données
    std::string path1 = path_fredo + "/Bati/Bati_Lyon5eme.shp";
    Shapefile essai1 = Shapefile(path1);
    std::string path2 = path_fredo + "/TronconRoute/TronconRoute_Lyon5eme.shp";
    Shapefile essai2 = Shapefile(path2);
    std::string db_name = "postgres";
    std::string db_user = "postgres";
    std::string db_password = "postgres";
    std::string db_host = "localhost";
    std::string db_port = "5432";
    essai1.import_to_db(db_name,db_user,db_password,db_host,db_port, 2154);
    essai2.import_to_db(db_name,db_user,db_password,db_host,db_port, 2154);

    //affichage du shapefile importé
    pqxx::connection c("user="+db_user+" password="+db_password+" host="+db_host+" port="+db_port+" dbname="+db_name+" target_session_attrs=read-write");
    pqxx::work k(c);
    pqxx::result rowbis = k.exec("SELECT ST_AsGeoJSON(geom) FROM "+essai1.getTableName()+";");
    QGraphicsItemGroup *layerGroup = essai1.plotShapefile(rowbis,scene);
    layerList[index] = new Layer("Layer "+QString::number(index), true, layerGroup);
    addLayerToListWidget(index, *layerList[index]);
    index++;

    pqxx::result rowter = k.exec("SELECT ST_AsGeoJSON(geom) FROM "+essai2.getTableName()+";");
    layerGroup =essai1.plotShapefile(rowter,scene);
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
