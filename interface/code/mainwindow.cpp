#include <iostream>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
#include <pqxx/pqxx>
#include "transformation.h"
#include <QColor>
#include "mainwindow.h"
#include "ui_mainwindow.h"
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

    /*_______________________________TEST______________________________________*/

    // Connect to the database

    // Créer une scène pour QGraphicsView
    scene = new QGraphicsScene(this);

    // Définir la scène pour le QGraphicsView
    ui->graphicsView_window2D->setScene(scene);

    /*_____________________________________________________________________________*/

    connect(ui->btn_zoomIn, &QPushButton::clicked, this, &MainWindow::OnButtonZoomIn);
    connect(ui->btn_zoomOut, &QPushButton::clicked, this, &MainWindow::OnButtonZoomOut);
    connect(ui->btn_switchMode2D3D, &QPushButton::clicked, this, &MainWindow::OnButtonSwitchTo2D3DClicked);
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
    //import d'un shapefile dans la base de données
    std::string path1 = "/home/formation/Documents/ProjetGeomatique/DONNEES_BDTOPO/Bati/Bati_Lyon5eme.shp";
    Shapefile essai1 = Shapefile(path1);
    std::string db_name = "essai_dbf";
    std::string db_user = "postgres";
    std::string db_password = "postgres";
    std::string db_host = "localhost";
    std::string db_port = "5432";
    essai1.import_to_db(db_name,db_user,db_password,db_host,db_port, 4326);

    std::cout<<essai1.getTableName()<<std::endl;
    //affichage du shapefile importé
    QColor colours[10] = {QColor("cyan"), QColor("magenta"), QColor("red"),
                          QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                          QColor("green"), QColor("darkGreen"), QColor("yellow"),
                          QColor("blue")};

    pqxx::connection c("user="+db_user+" password="+db_password+" host="+db_host+" port="+db_port+" dbname="+db_name+" target_session_attrs=read-write");
    pqxx::work k(c);
    pqxx::result rows = k.exec("SELECT id, ST_AsGeoJSON(geom) FROM "+essai1.getTableName()+";");
    // Parcourir toutes les lignes du résultat
    for (const auto& row : rows) {
        // Access columns by index
        auto id = row[0].as<std::string>();
        auto geojson = row[1].as<std::string>();

        srand(time(NULL));

        int nbgen=rand()%9+1;

        QColor myColor = colours[nbgen];

        // Utiliser la classe Transformation pour convertir le JSON en QPolygonF
        Transformation t;
        QPolygonF polygoneToPlot = t.JSONtoCoords(geojson);
        // Créer un objet pour le pol shp
        QGraphicsPolygonItem *polygoneToPlotItem = new QGraphicsPolygonItem(polygoneToPlot);
        polygoneToPlotItem->setBrush(myColor);

        ui->graphicsView_window2D->scene()->addItem(polygoneToPlotItem);
    }
    ui->graphicsView_window2D->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);


}

void MainWindow::OnButtonZoomIn()
{
    ui->graphicsView_window2D->scale(1.2,1.2);
    qreal currentScale = ui->graphicsView_window2D->transform().m11();

        // Parcourir tous les éléments de la scène
        for (QGraphicsItem* item : ui->graphicsView_window2D->scene()->items()) {
            QGraphicsPolygonItem* polyItem = dynamic_cast<QGraphicsPolygonItem*>(item);
            if (polyItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = polyItem->pen();
                pen.setWidthF(adjustedWidth);
                polyItem->setPen(pen);
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
            if (polyItem) {
                // Ajuster la largeur du trait en fonction du facteur de zoom
                qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                // Mettre à jour la largeur du trait
                QPen pen = polyItem->pen();
                pen.setWidthF(adjustedWidth);
                polyItem->setPen(pen);
            }
        }
}
