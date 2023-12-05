#include <iostream>
#include <pqxx/pqxx>
#include <stdlib.h>
#include <filesystem>
namespace fs = std::filesystem;

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QApplication>
#include <QWidget>
#include <QPointF>
#include <QCheckBox>
#include <QColor>
#include <QMouseEvent>
#include <QFileInfo>
#include <QCoreApplication>
#include <QThread>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"
#include "wfsdataflowwindow.h"
#include "wmsdataflowwindow.h"
#include "view_zoom.h"
#include "mntwindow.h"

#include "../src/2D/layer.h"
#include "../src/2D/transformation.h"
#include "../src/2D/shapefile.h"
#include "../src/2D/geotiff.h"
#include "../src/2D/rasteritem.h"
#include "../src/2D/rasterimport.h"
#include "../src/outils/dbmanager.h"
#include "../src/outils/docker.h"
#include "../src/2D/geojson.h"
#include "../src/2D/wfsflow.h"



//Initialisation du Docker
// Creating container
std::string pathDockerFile = "database-tisig";
Docker docker(pathDockerFile);
// Get the Ip Adress
const std::string ipAdress_d = docker.getIpAdress();


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    /*_______________________________Initialisation_________________________________________________________________________________________________*/

    ui->setupUi(this);

    // Initialisation in mode 2D //
    ui->stackedWidget->setCurrentIndex(mode);
        // Action 2D
    ui->action_add2DVectorLayer->setEnabled(!mode);
    ui->action_add2DRastorLayer->setEnabled(!mode);
    ui->action_add2DWFSDataFlow->setEnabled(!mode);
    ui->action_add2DWMSDataFlow->setEnabled(!mode);
        // Actions 3D
    ui->action_add3DVectorLayer->setEnabled(mode);
    ui->action_add3DRastorLayer->setEnabled(mode);
    ui->action_add3DModel->setEnabled(mode);
    ui->openGLWidget_window3D->setCamInfoDisplayer(this);
	    // Connect scene to QGraphicsView
	ui->graphicsView_window2D->setScene(scene);
	ui->graphicsView_window2D->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView_window2D->installEventFilter(this);
	View_zoom* z = new View_zoom(ui->graphicsView_window2D);
	z->set_modifiers(Qt::NoModifier);


    /*_______________________________Variables_________________________________________________________________________________________________*/

    // Creating scene for QGraphicsView
    scene = new QGraphicsScene(this);

	// Connect scene to QGraphicsView
	ui->graphicsView_window2D->setScene(scene);
	ui->graphicsView_window2D->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView_window2D->installEventFilter(this);


    // ip Address
    ipAdress = ipAdress_d;


    /*_______________________________Barre Gestion Couches et Aide_________________________________________________________________________________________________*/


    // Connecting help action
    connect(ui->action_help, &QAction::triggered, this, &MainWindow::OnActionHelpClicked);

    // Connecting add2dWFSdataflow action
    connect(ui->action_add2DWFSDataFlow, &QAction::triggered, this, &MainWindow::OnAction2DWFSDataFlowClicked);
    // Connecting add2dWMSdataflow action
    connect(ui->action_add2DWMSDataFlow, &QAction::triggered, this, &MainWindow::OnAction2DWMSDataFlowClicked);

    // Connecting add2dvectorlayer action and add3dvectorlayer action
    connect(ui->action_add2DVectorLayer, &QAction::triggered, this, &MainWindow::OnActionVector2DLayerClicked);
    connect(ui->action_add3DVectorLayer, &QAction::triggered, this, &MainWindow::OnActionVector3DLayerClicked);

    // Connecting add2drastorlayer action and add3drastorlayer action
    connect(ui->action_add2DRastorLayer, &QAction::triggered, this, &MainWindow::OnActionRastor2DLayerClicked);
    connect(ui->action_add3DRastorLayer, &QAction::triggered, this, &MainWindow::OnActionRastor3DLayerClicked);

    // Connecting add3dmodel action
    connect(ui->action_add3DModel, &QAction::triggered, this, &MainWindow::OnAction3DModelClicked);




    /*_______________________________Barre d'outils___________________________________________________________________________________________________*/


    // Connecting switch 2D/3D button
    connect(ui->btn_switchMode2D3D, &QPushButton::clicked, this, &MainWindow::OnButtonSwitchTo2D3DClicked);

    // Connecting ZoomIn button
    connect(ui->btn_zoomIn, &QPushButton::clicked, this, &MainWindow::OnButtonZoomIn);

    // Connecting ZoomOut button
    connect(ui->btn_zoomOut, &QPushButton::clicked, this, &MainWindow::OnButtonZoomOut);

    // Connecting ZoomOut button
    connect(ui->btn_zoomFull, &QPushButton::clicked, this, &MainWindow::OnButtonZoomFull);


    /*_______________________________Barre d'outils dans le gestionnaire de couches___________________________________________________________________________________________________*/


    // Connecter le bouton "Up" à la fonction de déplacement vers le haut
    connect(ui->btn_moveLayerUp2D, &QPushButton::clicked, [=]() {
        moveItemUp();
    });

    // Connecter le bouton "Down" à la fonction de déplacement vers le bas
    connect(ui->btn_moveLayerDown2D, &QPushButton::clicked, [=]() {
        moveItemDown();
    });

    // Connecter le bouton "Poubelle" à la fonction de suppression
    connect(ui->btn_deleteLayer2D, &QPushButton::clicked, [=]() {
        onButtonClickedDeleteLayer();
    });

    // Connecter le bouton "Zoom sur la couche" à la fonction de zoom sur la couche
    connect(ui->btn_zoomOnLayer2D, &QPushButton::clicked, [=]() {
        onButtonClickedZoomOnLayer();
    });
}

MainWindow::~MainWindow()
{
    // Empty wfs flow folder
    for (const auto& entry : fs::directory_iterator("data/wfsFlow/")) {
        fs::remove_all(entry.path());
    }

    // Delete all layers
    for(auto pair: layerList)
    {
        delete pair.second;
    }

	// Delete all items from 2D window
	for (QGraphicsItem* item : ui->graphicsView_window2D->scene()->items())
	{
	delete item;
	}
	
	// Delete scene
	delete scene;
	
	// Delete interface
	delete ui;
	
	//Delete shapefiles
	for (std::pair <const int, Shapefile * > truc : ShpList){
	Shapefile* shp = truc.second;
		shp->~Shapefile();
	}
	
	//Empty the symbologie table
	DbManager test("database2D", ipAdress);
	std::string request = "TRUNCATE TABLE symbologie";
	test.Request(request);
}


Ui::MainWindow * MainWindow::getUi() const
{
    return ui;
}

void MainWindow::OnButtonSwitchTo2D3DClicked()
{
    mode = !mode;
    ui->stackedWidget->setCurrentIndex(mode);

    ui->stackedWidget_Rightside->setCurrentIndex(mode);

    ui->action_add2DVectorLayer->setEnabled(!mode);
    ui->action_add2DRastorLayer->setEnabled(!mode);
    ui->action_add2DWFSDataFlow->setEnabled(!mode);
    ui->action_add2DWMSDataFlow->setEnabled(!mode);

    ui->action_add3DVectorLayer->setEnabled(mode);
    ui->action_add3DRastorLayer->setEnabled(mode);
    ui->action_add3DModel->setEnabled(mode);

    ui->tableWidget_layerAttributeInformation2D->horizontalHeader()->setVisible(1);
    ui->tableWidget_layerAttributeInformation3D->horizontalHeader()->setVisible(1);

    // when hidden, camera controls are disabled
    if (!mode)
        ui->openGLWidget_window3D->hide();
    else
        ui->openGLWidget_window3D->show();

    this->update();

    ui->stackedWidget->update();
    ui->stackedWidget_Rightside->update();
    ui->graphicsView_window2D->update();
    ui->openGLWidget_window3D->update();

    this->repaint();

    ui->stackedWidget->repaint();
    ui->stackedWidget_Rightside->repaint();
    ui->graphicsView_window2D->repaint();
    ui->openGLWidget_window3D->repaint();
}

void MainWindow::OnActionHelpClicked()
{
    HelpWindow helpwindow;
    helpwindow.setModal(true);
    helpwindow.exec();
}

void MainWindow::OnAction2DWFSDataFlowClicked()
{
    WFSDataFlowWindow wfsdataflowwindow;
    wfsdataflowwindow.setModal(true);
    int result = wfsdataflowwindow.exec();
    if(result==QDialog::Accepted){
        std::string url = wfsdataflowwindow.getURL();

        // Emprise de la fenêtre à récupérer
        std::string longmin = std::to_string(45.727093);
        std::replace(longmin.begin(), longmin.end(), ',', '.');
        std::string latmin = std::to_string(4.819074);
        std::replace(latmin.begin(), latmin.end(), ',', '.');
        std::string longmax = std::to_string(45.746508);
        std::replace(longmax.begin(), longmax.end(), ',', '.');
        std::string latmax = std::to_string(4.850961);
        std::replace(latmax.begin(), latmax.end(), ',', '.');

        QString wfsDataSource = QString::fromStdString(url+"&REQUEST=GetFeature&OUTPUTFORMAT=SHAPE-ZIP&BBOX="+longmin+","+latmin+","+longmax+','+latmax+"&SRSNAME=EPSG:2154");
        std::cout<<wfsDataSource.toStdString()<<std::endl;
        WFSFlow *wfsflow = new WFSFlow(wfsDataSource);
        wfsflow->downloadZIP();
        std::string PathWfsFlow = wfsflow->filePath.toStdString();
        PathWfsFlow.replace(PathWfsFlow.size() - 4, 4, ".shp");
        std::cout << "MAINWINDOW RETURN ========> " << PathWfsFlow<<std::endl;
        // Boucle tant que le fichier n'existe pas
        while (!QFileInfo(QString::fromStdString(PathWfsFlow)).exists()) {
            QThread::msleep(100); // Pause de 100 millisecondes
            QCoreApplication::processEvents();
        }

        // Le fichier existe maintenant, vous pouvez appeler AddShpFileClicked
        AddShpFileClicked(PathWfsFlow);
//        AddShpFileClicked(PathWfsFlow);
    }
}



void MainWindow::OnAction2DWMSDataFlowClicked()
{
    WMSDataFlowWindow wmsdataflowwindow;
    wmsdataflowwindow.setModal(true);
    int result = wmsdataflowwindow.exec();
    if(result==QDialog::Accepted){
        std::cout << wmsdataflowwindow.getLien()<<std::endl;
    }
}

std::string MainWindow::OnActionVector2DLayerClicked()
{
    QString fileNameVectorLayer = QFileDialog::getOpenFileName(this, tr("Ouvrir une couche de données vecteur"), "../../../", tr("Fichier vecteur (*.shp *.geojson)"));
    std::string path = fileNameVectorLayer.toStdString();
    std::string shp = "shp";
    std::string json = "geojson";
    if (path != ""){
        if (path.size() >= shp.size() &&
                path.compare(path.size() - shp.size(), shp.size(), shp) == 0){

            this->AddShpFileClicked(path);
        }
        else if (path.size() >= json.size() &&
                 path.compare(path.size() - json.size(), json.size(), json) == 0){
            GeoJson file = GeoJson(path);
            file.ConvertToShp();
            this->AddShpFileClicked(file.GetShpFilePath());
        }
    }
    return path;
}

std::string MainWindow::OnActionVector3DLayerClicked()
{

    QString fileNameVectorLayer = QFileDialog::getOpenFileName(this, tr("Ouvrir une couche de données vecteur"), "../../../", tr("Fichier vecteur (*.shp *.geojson)"));
    std::string path = fileNameVectorLayer.toStdString();
    return path;

}

std::string MainWindow::OnActionRastor2DLayerClicked()
{

	QString fileNameRastorLayer = QFileDialog::getOpenFileName(this, tr("Ouvrir une couche de données raster"), "../../../", tr("GeoTIFF (*.tif *.TIF *.tiff)"));
	std::string path = fileNameRastorLayer.toStdString();
    if (path != ""){
        this->AddGeotiffFileClicked(path);
    }
    return path;
}

void MainWindow::OnActionRastor3DLayerClicked()
{
    MntWindow mntwindow;
    mntwindow.setModal(true);
    mntwindow.exec();
}



std::string MainWindow::OnAction3DModelClicked()
{
    QString fileName3Dmodel = QFileDialog::getOpenFileName(this, tr("Ouvrir un modèle 3D"), "../../../", tr("CityGML (*.gml)"));
    return fileName3Dmodel.toStdString();
}


void MainWindow::Display3DCameraCoordinates(QVector3D camPosition)
{
    std::string camPositionX = std::to_string((int)(camPosition.x()));
    std::string camPositionY = std::to_string((int)(camPosition.y()));
    std::string camPositionZ = std::to_string((int)(camPosition.z()));

    QString textN = QString::fromStdString(camPositionY);
    QString textE = QString::fromStdString(camPositionX);
    QString textH = QString::fromStdString(camPositionZ);

    ui->lineEdit_coords3DN->setText(textN);
    ui->lineEdit_coords3DE->setText(textE);
    ui->lineEdit_coords3DH->setText(textH);
}

void MainWindow::Display3DZoomLevel(float zoom)
{
    QString text = QString::fromStdString(std::to_string((int)(zoom*100)));
    ui->lineEdit_zoom3D->setText(text+"%");
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
    Shapefile * essai1 = new Shapefile(path, test);

    essai1->import_to_db(2154);
    QColor myColor = essai1->showColor();

    int layerId = essai1->getId();

	//affichage des shapefiles importé
	test.Request("SELECT ST_AsGeoJSON(geom) FROM "+essai1->getTableName()+";");
	pqxx::result rowbis =test.getResult();
	QGraphicsItemGroup *layerGroup = essai1->plotShapefile(rowbis,scene, myColor);
    ui->lineEdit_epsg2D->setText(essai1->getEPSGtoSet());
    layerList[layerId] = new Layer("Layer "+QString::number(index)+ " : "+ QString(essai1->getTableName().c_str()), true, layerGroup);
    addLayerToListWidget(layerId, *layerList[layerId]);
    index++;

    ShpList.insert(std::pair<const int, Shapefile *>(layerId, essai1));
}


void MainWindow::AddGeotiffFileClicked(std::string path)
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

    //Import raster from the file system into the DB

	Geotiff geotiff(path);
    std::cout << "geotiff ouverte" << std::endl;

    geotiff.WriteGeotiffAndMetadataToPostgis(test);
    std::cout << "geotiff ecrit dans la bdd" << std::endl;


    //Import raster from the DB into a layer

    QString filePath = QString::fromStdString(path);
    RasterItem* rasterItem = RasterImport::CreateRasterItemFromDb(filePath,test);

    QGraphicsItemGroup *layerGroup = new QGraphicsItemGroup();
    scene->addItem(layerGroup);
    layerGroup->addToGroup(rasterItem);

    int layerId = rasterItem->getId();

    size_t lastSlash = path.find_last_of("/\\");
    std::string fileName = path.substr(lastSlash + 1);
    fileName = fileName.substr(0, fileName.find_last_of("."));

    layerList[layerId] = new Layer("Layer "+QString::number(index)+" : "+QString::fromStdString(fileName), true, layerGroup);

    addLayerToListWidget(layerId, *layerList[layerId]);
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

void MainWindow::addLayerToListWidget(int layerId, Layer &layer) {


    // Initie le zIndex de la couche
    layer.setZIndex(index);

    // Créez un nouvel élément pour la couche
    layer.layerItem = new QListWidgetItem(ui->listeWidget_layersList2D);
    layer.layerItem->setData(Qt::UserRole, layerId);

    // Créez un widget personnalisé pour cet élément (contenant un label et une case à cocher)
    layer.layerWidget = new QWidget();
    layer.layout = new QHBoxLayout(layer.layerWidget);


    layer.visibilityCheckbox = new QCheckBox("");
    layer.visibilityCheckbox->setChecked(layer.isLayerVisible());
    layer.layerLabel = new QLabel(layer.getLayerName());

    // Connectez le signal clicked de la case à cocher à une fonction pour gérer la visibilité
    connect(layer.visibilityCheckbox, &QCheckBox::toggled, [=](bool checked) {
        layerList[layerId]->getLayerGroup()->setVisible(checked);
        layerList[layerId]->setLayerVisible(checked);
    });


    layer.layout->addWidget(layer.visibilityCheckbox);
    layer.layout->addWidget(layer.layerLabel);
    layer.layout->setAlignment(Qt::AlignLeft);

    layer.layout->setContentsMargins(2, 0, 2, 0); // Taille de la ligne de la couche dans le gestionnaire
    layer.layout->setSpacing(10); // Écart entre la checkbox et le nom de la couche

    layer.layerWidget->setLayout(layer.layout);
    layer.layerItem->setSizeHint(layer.layerWidget->sizeHint());



    ui->listeWidget_layersList2D->setItemWidget(layer.layerItem, layer.layerWidget);

    // met à jour l'ordre de superpositions des couches
    updateLayerOrderInGraphicsView();
}

void MainWindow::moveItemDown() {
    QListWidgetItem *item = ui->listeWidget_layersList2D->currentItem();
    int currentIndex = ui->listeWidget_layersList2D->row(item);

    if (item && currentIndex < ui->listeWidget_layersList2D->count()-1)
    {
        // Change la profondeur des couches
        int currentId = item->data(Qt::UserRole).toInt();
        QListWidgetItem *nextItem = ui->listeWidget_layersList2D->item(currentIndex+1);
        int nextId = nextItem->data(Qt::UserRole).toInt();

        int currentZIndex = layerList[currentId]->getZIndex();
        int prevZIndex = layerList[nextId]->getZIndex();

        layerList[currentId]->setZIndex(prevZIndex);
        layerList[nextId]->setZIndex(currentZIndex);

        updateLayerOrderInGraphicsView();

        // Change l'ordre dans la liste des couches
        QWidget *itemWidget = ui->listeWidget_layersList2D->itemWidget(item);
        QWidget *tempWidget = new QWidget();
        QLayout *widgetLayout = itemWidget->layout();
        tempWidget->setLayout(widgetLayout);

        layerList[currentId]->layerWidget = tempWidget;

        QListWidgetItem *currentItem = ui->listeWidget_layersList2D->takeItem(currentIndex);

        ui->listeWidget_layersList2D->insertItem(currentIndex+1, currentItem);
        ui->listeWidget_layersList2D->setItemWidget(currentItem, tempWidget);
        ui->listeWidget_layersList2D->setCurrentRow(currentIndex+1);
    }
}

void MainWindow::moveItemUp() {
    QListWidgetItem *item = ui->listeWidget_layersList2D->currentItem();
    int currentIndex = ui->listeWidget_layersList2D->row(item);

    if (item && currentIndex > 0) {

        // Change la profondeur des couches
        int currentId = item->data(Qt::UserRole).toInt();
        QListWidgetItem *prevItem = ui->listeWidget_layersList2D->item(currentIndex-1);
        int prevId = prevItem->data(Qt::UserRole).toInt();

        int currentZIndex = layerList[currentId]->getZIndex();
        int prevZIndex = layerList[prevId]->getZIndex();

        layerList[currentId]->setZIndex(prevZIndex);
        layerList[prevId]->setZIndex(currentZIndex);

        updateLayerOrderInGraphicsView();

        // Change l'ordre dans la liste des couches
        QWidget *itemWidget = ui->listeWidget_layersList2D->itemWidget(item);
        QWidget *tempWidget = new QWidget();
        QLayout *widgetLayout = itemWidget->layout();
        tempWidget->setLayout(widgetLayout);

        layerList[currentId]->layerWidget = tempWidget;

        QListWidgetItem *currentItem = ui->listeWidget_layersList2D->takeItem(currentIndex);

        ui->listeWidget_layersList2D->insertItem(currentIndex-1, currentItem);
        ui->listeWidget_layersList2D->setItemWidget(currentItem, tempWidget);
        ui->listeWidget_layersList2D->setCurrentRow(currentIndex-1);
    }
}

void MainWindow::updateLayerOrderInGraphicsView() {
    for(auto pair: layerList)
    {
        pair.second->getLayerGroup()->setZValue(pair.second->getZIndex());
    }
    ui->graphicsView_window2D->repaint();
}

void MainWindow::onButtonClickedDeleteLayer()
{
    QListWidgetItem *item = ui->listeWidget_layersList2D->currentItem();


    if (item)
    {
        int layerId = item->data(Qt::UserRole).toInt();
        ui->listeWidget_layersList2D->removeItemWidget(item);
        delete item;

        ui->graphicsView_window2D->scene()->removeItem(layerList[layerId]->getLayerGroup());
        delete layerList[layerId]->getLayerGroup();
        layerList.erase(layerId);
    }
}

void MainWindow::onButtonClickedZoomOnLayer()
{
    QListWidgetItem *item = ui->listeWidget_layersList2D->currentItem();

    if (item)
    {
        int layerId = item->data(Qt::UserRole).toInt();
        QGraphicsItemGroup *layer = layerList[layerId]->getLayerGroup();
        if (layer->isVisible())
        {
            QRectF emprise = layer->sceneBoundingRect();
            ui->graphicsView_window2D->fitInView(emprise, Qt::KeepAspectRatio);

            qreal currentScale = ui->graphicsView_window2D->transform().m11();

            // Parcourir tous les éléments de la scène
            for (QGraphicsItem* item : ui->graphicsView_window2D->scene()->items())
            {
                QGraphicsPolygonItem* polyItem = dynamic_cast<QGraphicsPolygonItem*>(item);
                QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
                QGraphicsEllipseItem* pointItem = dynamic_cast<QGraphicsEllipseItem*>(item);

                if (polyItem)
                {
                    // Ajuster la largeur du trait en fonction du facteur de zoom
                    qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                    // Mettre à jour la largeur du trait
                    QPen pen = polyItem->pen();
                    pen.setWidthF(adjustedWidth);
                    polyItem->setPen(pen);
                }


                if (lineItem)
                {
                    // Ajuster la largeur du trait en fonction du facteur de zoom
                    qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                    // Mettre à jour la largeur du trait
                    QPen pen = lineItem->pen();
                    pen.setWidthF(adjustedWidth);
                    lineItem->setPen(pen);
                }

                if (pointItem)
                {
                    // Ajuster la largeur du trait en fonction du facteur de zoom
                    qreal adjustedWidth = 2.0 / currentScale; // Remplacez 2.0 par l'épaisseur de trait de référence

                    // Mettre à jour la largeur du trait
                    QPen pen = pointItem->pen();
                    pen.setWidthF(adjustedWidth);
                    pointItem->setPen(pen);
                }
            }
        }
    }
}

void MainWindow::getAttributesLayer(QMouseEvent *event){
    if (ui->listeWidget_layersList2D->currentItem())
    {
        //Get selected shapefile
        QListWidgetItem *item = ui->listeWidget_layersList2D->currentItem();
        int currentId = item->data(Qt::UserRole).toInt();
        Shapefile * shp = ShpList[currentId];

        QPointF mousePos = ui->graphicsView_window2D->mapToScene(event->pos());
        ui->tableWidget_layerAttributeInformation2D->clear();
        ui->tableWidget_layerAttributeInformation2D->setRowCount(0);
        QStringList nameCol;
        nameCol << "Nom" << "Valeur";
        ui->tableWidget_layerAttributeInformation2D->setHorizontalHeaderLabels(nameCol);
        double x = mousePos.x();
        double y = -mousePos.y();  // Assurez-vous du sens de l'axe y en fonction de votre scène
        std::string x_str = std::to_string(x);
        std::string y_str = std::to_string(y);
        std::replace(x_str.begin(), x_str.end(), ',', '.');
        std::replace(y_str.begin(), y_str.end(), ',', '.');
        //std::cout << "Les coordonnées écran : " << x_str << ", " << y_str << std::endl;



        DbManager db_manager = shp->getDbManager();
        std::string dataType = shp->getDataType();
        std::string request;
        if (dataType == "Polygon"){
            request = "SELECT * FROM "+shp->getTableName()+" WHERE ST_Within(ST_SetSRID(ST_MakePoint(" + x_str + "," + y_str + "), 2154), geom);";
        }
        else if(dataType == "LineString" || dataType == "MultiLineString"){
            request = "SELECT * FROM "+shp->getTableName()+" WHERE ST_Distance(ST_SetSRID(ST_MakePoint(" + x_str + "," + y_str + "), 2154), geom) < 3 ORDER BY ST_Distance(ST_SetSRID(ST_MakePoint(" + x_str + "," + y_str + "), 2154), geom) LIMIT 1;";
        }
        db_manager.Request(request);
        pqxx::result rows_shape = db_manager.getResult();

        if (!rows_shape.empty()){
            for (pqxx::result::const_iterator row = rows_shape.begin(); row != rows_shape.end(); ++row) {
                for (int j = 0; j < row.size(); ++j) {
                    std::string name_col = rows_shape.column_name(j);
                    if (!row[j].is_null()) {
                        std::string value = row[j].as<std::string>();

                        // Ajoute une nouvelle ligne pour chaque colonne
                        ui->tableWidget_layerAttributeInformation2D->insertRow(j);

                        // Remplit la première colonne avec le nom de la colonne
                        QTableWidgetItem *col_name_item = new QTableWidgetItem(QString::fromStdString(name_col));
                        ui->tableWidget_layerAttributeInformation2D->setItem(j, 0, col_name_item);

                        // Remplit la deuxième colonne avec la valeur
                        QTableWidgetItem *value_item = new QTableWidgetItem(QString::fromStdString(value));
                        ui->tableWidget_layerAttributeInformation2D->setItem(j, 1, value_item);
                    } else {
                        //std::cout << "Nom de la colonne : " << name_col << ", Valeur : NULL" << std::endl;
                        // Ajoute une nouvelle ligne pour chaque colonne
                        ui->tableWidget_layerAttributeInformation2D->insertRow(j);

                        // Remplit la première colonne avec le nom de la colonne
                        QTableWidgetItem *col_name_item = new QTableWidgetItem(QString::fromStdString(name_col));
                        ui->tableWidget_layerAttributeInformation2D->setItem(j, 0, col_name_item);

                        // Remplit la deuxième colonne avec la valeur
                        QTableWidgetItem *value_item = new QTableWidgetItem(QString::fromStdString("NULL"));
                        ui->tableWidget_layerAttributeInformation2D->setItem(j, 1, value_item);
                    }
                }
            }
        } else {
            std::cerr << "Aucune ligne trouvée." << std::endl;
        }
        QMainWindow::mousePressEvent(event);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->graphicsView_window2D && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        this->getAttributesLayer(mouseEvent);
        std::cout << "EVENT FILTER" << std::endl;
        return true;
    }

    return QObject::eventFilter(obj, event);
}
