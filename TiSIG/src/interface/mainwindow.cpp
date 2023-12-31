#include <iostream>
#include <pqxx/pqxx>
#include <stdlib.h>
#include <algorithm>
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
#include <QSlider>
#include <QAbstractSlider>
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

#include "layermanager3d.h"

#include "../src/2D/layer.h"
#include "../src/2D/transformation.h"
#include "../src/2D/shapefile.h"
#include "../src/2D/geotiff.h"
#include "../src/2D/rasteritem.h"
#include "../src/2D/rasterimport.h"
#include "../src/outils/dbmanager.h"
#include "../src/outils/docker.h"
#include "../src/2D/geojson.h"
#include "../src/3D/batiments.h"
#include "../src/3D/exempleobject3dstorage.h"
#include "../src/3D/mnt3dstorage.h"
#include "../src/2D/wfsflow.h"
#include "../src/2D/crs_converter.hpp"
#include "../src/2D/shpwfsflow.h"


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
	ui->openGLWidget_window3D->setPickingInfoDisplayer(this);

	ui->tableWidget_layerAttributeInformation2D->horizontalHeader()->setVisible(1);
	ui->tableWidget_layerAttributeInformation3D->horizontalHeader()->setVisible(1);



	/*_______________________________Variables_________________________________________________________________________________________________*/

	// Creating scene for QGraphicsView
	scene = new QGraphicsScene(this);

	// Connect scene to QGraphicsView
	ui->graphicsView_window2D->setScene(scene);
	ui->graphicsView_window2D->setDragMode(QGraphicsView::ScrollHandDrag);
	ui->graphicsView_window2D->installEventFilter(this);
	View_zoom* z = new View_zoom(ui->graphicsView_window2D);
	z->set_modifiers(Qt::NoModifier);


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
//	connect(ui->action_add3DModel, &QAction::triggered, this, &MainWindow::OnAction3DModelClicked);


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

	// Connecter le bouton "Up" à la fonction de déplacement vers le haut
	connect(ui->btn_moveLayerUp3D, &QPushButton::clicked, [=]() {
		moveItemUp();
	});

	// Connecter le bouton "Down" à la fonction de déplacement vers le bas
	connect(ui->btn_moveLayerDown3D, &QPushButton::clicked, [=]() {
		moveItemDown();
	});

	connect(ui->slider3D, &QSlider::valueChanged, this, &MainWindow::getValueFromSlider);
	this->getValueFromSlider();

	/*--------------------- example of linking storage to 3D interface --------------------*/
	//Création du gestionnaire de liste 3D
	this->layerList3D = new LayerManager3D(this->getUi());
	connect(ui->action_add3DModel, &QAction::triggered, this, &MainWindow::testAdd3DModel); // à modifier ligne 108


    this->storage3D = new Batiments(ipAdress);
    this->layer3D = new Layer3D(this->storage3D, 1, "Batiment 5e Lyon");
    this->ui->openGLWidget_window3D->addLayer(0, this->layer3D);
}

MainWindow::~MainWindow()
{
	/* ------------------------------   2D deletion   ----------------------- */

	// Empty WFS flow folder
	for (const auto& entry : fs::directory_iterator("data/wfsFlow/")) {
		fs::remove_all(entry.path());
	}

	// Empty WMS flow folder
	for (const auto& entry : fs::directory_iterator("data/wmsFlow/")) {
		fs::remove_all(entry.path());
	}

	// Empty WMTS flow folder
	for (const auto& entry : fs::directory_iterator("data/wmtsFlow/")) {
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

	//Delete shapefiles
	for (std::pair <const int, Shapefile * > truc : ShpList){
	Shapefile* shp = truc.second;
		shp->~Shapefile();
	}

	//Empty the symbologie table
	DbManager test("database2D", ipAdress);
	std::string request = "DROP TABLE IF EXISTS symbologie_shp";
	test.CreateTable(request);
	request = "DROP TABLE IF EXISTS symbologie_flow";
	test.CreateTable(request);

	/* ------------------------------   3D deletion   ----------------------- */
	delete this->storage3D;
	delete this->layer3D;

	/* ------------------------------ other deletions ----------------------- */
	// Delete interface
	delete ui;

}


Ui::MainWindow * MainWindow::getUi() const
{
	return ui;
}


float MainWindow::getValueFromSlider()
{
	float val = ui->slider3D->value()/100.;
	this->ui->openGLWidget_window3D->setSymbologyOpacity(val);
	return val;
};


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


QRectF MainWindow::get2DViewExtent()
{
	if (index == 0)
	{
		QRectF defaultExtent = QRectF(QPointF(837354.3000,-6520755.6000),QPointF(842550.9000,-6517267.8723));
		return defaultExtent;
	}
	QRectF viewSceneRect = ui->graphicsView_window2D->mapToScene(ui->graphicsView_window2D->rect()).boundingRect();
	return viewSceneRect;
}


std::vector<std::string> MainWindow::getExtentWMS()
{
	double Xmin = static_cast<double>(std::abs(get2DViewExtent().topLeft().x()));
	double Ymax = static_cast<double>(std::abs(get2DViewExtent().topLeft().y()));
	std::vector<double> coordL = crs_converter(2154, Xmin, Ymax, 4326);

	double Xmax = static_cast<double>(std::abs(get2DViewExtent().bottomRight().x()));
	double Ymin = static_cast<double>(std::abs(get2DViewExtent().bottomRight().y()));
	std::vector<double> coordR = crs_converter(2154, Xmax, Ymin, 4326);

	// Emprise de la fenêtre à récupérer
	std::string latmin = std::to_string(coordR[1]);
	std::replace(latmin.begin(), latmin.end(), ',', '.');
	std::string longmin = std::to_string(coordL[0]);
	std::replace(longmin.begin(), longmin.end(), ',', '.');
	std::string latmax = std::to_string(coordL[1]);
	std::replace(latmax.begin(), latmax.end(), ',', '.');
	std::string longmax = std::to_string(coordR[0]);
	std::replace(longmax.begin(), longmax.end(), ',', '.');

	std::vector<std::string> extent = {latmin, longmin, latmax, longmax};

	return extent;
}
void MainWindow::OnAction2DWFSDataFlowClicked()
{
	WFSDataFlowWindow wfsdataflowwindow;
	wfsdataflowwindow.setModal(true);
	int result = wfsdataflowwindow.exec();
	if(result==QDialog::Accepted){
		std::vector<std::string> extent = getExtentWMS();
		std::string latmin = extent[0];
		std::string longmin = extent[1];
		std::string latmax = extent[2];
		std::string longmax = extent[3];
		std::string url = wfsdataflowwindow.getURL();

		WFSFlow *wfsflow = new WFSFlow(url, latmin, longmin, latmax, longmax);
		std::string PathWfsFlow = wfsflow->GetfilePath();
//        PathWfsFlow.replace(PathWfsFlow.size() - 4, 4, ".shp");
//        std::cout << "Chemin du shp : " << PathWfsFlow<<std::endl;

		if (fs::exists(PathWfsFlow)) {
			std::cerr << "Ce flux est déjà disponible." << std::endl;
			return;
		}

		else {
			wfsflow->downloadZIP();
			// Boucle tant que le fichier n'existe pas
			while (!QFileInfo(QString::fromStdString(PathWfsFlow)).exists()) {
				QThread::msleep(100); // Pause de 100 millisecondes
				QCoreApplication::processEvents();
			}
			// Le fichier existe maintenant, vous pouvez appeler AddShpFileClicked
			AddshpWFS(wfsflow);
		}

	}
}


//fonction update avec des listenners :
//    // Parcourir le répertoire et supprimer tous les fichiers correspondant à la couche
//    for (const auto& entry : fs::directory_iterator("data/wfsFlow/")) {
//        if (entry.path().filename().stem().string() == fs::path(PathWfsFlow).filename().stem().string()) {
//            fs::remove(entry.path());
//        }
//    }


void MainWindow::AddshpWFS(WFSFlow *wfsflow)
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
	 shpWFSflow *shpFlow = new shpWFSflow(test, wfsflow);

	shpFlow->import_to_db(2154);
	QColor myColor = shpFlow->showColor();

	int layerId = shpFlow->getId();

	//affichage des shapefiles importé
	test.Request("SELECT ST_AsGeoJSON(geom) FROM "+shpFlow->getTableName()+";");
	pqxx::result rowbis =test.getResult();
	test.Request("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '"+shpFlow->getTableName()+"';");
	pqxx::result columnNamesResult = test.getResult();
	std::vector<std::string> columnList;

	for (const auto& row : columnNamesResult)
	{
		std::string columnName = row[0].as<std::string>();
		columnList.push_back(columnName);
	}

	pqxx::result rowbisType;
	pqxx::result rowTer;


	if (std::find(columnList.begin(), columnList.end(), "nature") != columnList.end())
	{
	  test.Request("SELECT nature From "+shpFlow->getTableName()+";");
	  rowbisType = test.getResult();
	  test.Request("SELECT DISTINCT nature FROM "+shpFlow->getTableName()+";");
	  rowTer = test.getResult();
	}


	QGraphicsItemGroup *layerGroup = shpFlow->plotShapefile(rowbis, rowbisType, rowTer,scene, myColor);
	ui->lineEdit_epsg2D->setText(shpFlow->getEPSGtoSet());
	layerList[layerId] = new Layer("Layer "+QString::number(index)+ " : "+ QString(shpFlow->getTableName().c_str()), true, layerGroup);
	addLayerToListWidget(layerId, *layerList[layerId]);
	index++;

	ShpList.insert(std::pair<const int, Shapefile *>(layerId, shpFlow));
}


void MainWindow::OnAction2DWMSDataFlowClicked()
{
	WMSDataFlowWindow wmsdataflowwindow;
	wmsdataflowwindow.setModal(true);
	int result = wmsdataflowwindow.exec();
	if(result==QDialog::Accepted){
		std::string url = wmsdataflowwindow.getURL();
		std::cout << url<<std::endl;
		// code Axel
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
	int result = mntwindow.exec();

	if (result == QDialog::Accepted) {
		std::cout << mntwindow.getObj()<< std::endl;

		this->layerList3D->addLayer3DtoOpenGLWidgetAndListWidget(
//                    new MNT3dstorage(mntwindow.getObj(), mntwindow.getXTranslate(), mntwindow.getYTranslate(), mntwindow.getTexture())
					new MNT3dstorage(mntwindow.getObj(), 0.0, 0.0, mntwindow.getTexture())
					);
//        std::cout<<mntwindow.getXTranslate()<<" "<<mntwindow.getYTranslate()<<std::endl;
//        this->storage3D = new MNT3dstorage(mntwindow.getObj(), mntwindow.getTexture());
//		this->layer3D = new Layer3D(this->storage3D);
//		this->ui->openGLWidget_window3D->addLayer(1, this->layer3D);
	}

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

void MainWindow::Display3DPickingResult(const std::map<std::string, std::string> &data)
{
	// reset display
	ui->tableWidget_layerAttributeInformation3D->clear();
	ui->tableWidget_layerAttributeInformation3D->setRowCount(0);

	// adding header
	QStringList nameCol;
	nameCol << "Nom" << "Valeur";
	ui->tableWidget_layerAttributeInformation3D->setHorizontalHeaderLabels(nameCol);

	int j = 0;
	for (auto &pair: data)
	{
		ui->tableWidget_layerAttributeInformation3D->insertRow(j);

		QTableWidgetItem *col_name_item = new QTableWidgetItem(QString::fromStdString(pair.first));
		ui->tableWidget_layerAttributeInformation3D->setItem(j, 0, col_name_item);

		QTableWidgetItem *value_item = new QTableWidgetItem(QString::fromStdString(pair.second));
		ui->tableWidget_layerAttributeInformation3D->setItem(j, 1, value_item);

		j +=1;
	}
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
	test.Request("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '"+essai1->getTableName()+"';");
	pqxx::result columnNamesResult = test.getResult();
	std::vector<std::string> columnList;

	for (const auto& row : columnNamesResult)
	{
		std::string columnName = row[0].as<std::string>();
		columnList.push_back(columnName);
	}

	pqxx::result rowbisType;
	pqxx::result rowTer;


	if (std::find(columnList.begin(), columnList.end(), "nature") != columnList.end())
	{
	  test.Request("SELECT nature From "+essai1->getTableName()+";");
	  rowbisType = test.getResult();
	  test.Request("SELECT DISTINCT nature FROM "+essai1->getTableName()+";");
	  rowTer = test.getResult();
	}


	QGraphicsItemGroup *layerGroup = essai1->plotShapefile(rowbis, rowbisType, rowTer,scene, myColor);
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
	if (!this->mode)
	{
		qreal currentScale = ui->graphicsView_window2D->transform().m11();
		if (currentScale<250){
			ui->graphicsView_window2D->scale(1.2,1.2);
		}
	}
	else
	{
		this->ui->openGLWidget_window3D->ZoomIn();
	}
}

void MainWindow::OnButtonZoomOut()
{
	if (!this->mode) {
		ui->graphicsView_window2D->scale(1/1.2,1/1.2);
	}
	else
	{
		this->ui->openGLWidget_window3D->ZoomOut();
	}
}

void MainWindow::OnButtonZoomFull()
{
	if (!this->mode) {
		// Zoom sur l'ensemble des couches visibles
		QRectF visibleItemsRect;
		foreach (QGraphicsItem *item, scene->items()) {
			if (item->isVisible())
				visibleItemsRect = visibleItemsRect.united(item->sceneBoundingRect());
		}

		ui->graphicsView_window2D->fitInView(visibleItemsRect,Qt::KeepAspectRatio);
		qreal currentScale = ui->graphicsView_window2D->transform().m11();
		std::cout<<currentScale<<std::endl;
	} else {
		this->ui->openGLWidget_window3D->ZoomAtEmprise();
	}
}

void MainWindow::addLayerToListWidget(int layerId, Layer &layer) {


	// Initie le zIndex de la couche
	layer.setZIndex(index);

	// Créez un nouvel élément pour la couche
	layer.layerItem = new QListWidgetItem();

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


	ui->listeWidget_layersList2D->insertItem(0, layer.layerItem);
	ui->listeWidget_layersList2D->setItemWidget(layer.layerItem, layer.layerWidget);

	// met à jour l'ordre de superpositions des couches
	updateLayerOrderInGraphicsView();
}

void MainWindow::moveItemDown() {

	if (!this->mode)
	{
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
	else
	{
		QListWidgetItem *item = ui->listeWidget_layersList3D->currentItem();
		int currentIndex = ui->listeWidget_layersList3D->row(item);

		if (item && currentIndex < ui->listeWidget_layersList3D->count()-1)
		{

			// Change l'ordre dans la liste des couches
			int currentId = item->data(Qt::UserRole).toInt();

			QWidget *itemWidget = ui->listeWidget_layersList3D->itemWidget(item);
			QWidget *tempWidget = new QWidget();
			QLayout *widgetLayout = itemWidget->layout();
			tempWidget->setLayout(widgetLayout);

			layerList3D->getLayer3D(currentId)->setLayerWidget(tempWidget);

			QListWidgetItem *currentItem = ui->listeWidget_layersList3D->takeItem(currentIndex);

			ui->listeWidget_layersList3D->insertItem(currentIndex+1, currentItem);
			ui->listeWidget_layersList3D->setItemWidget(currentItem, tempWidget);
			ui->listeWidget_layersList3D->setCurrentRow(currentIndex+1);
		}
	}
}

void MainWindow::moveItemUp() {

	if (!this->mode)
	{
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
	else
	{
		QListWidgetItem *item = ui->listeWidget_layersList3D->currentItem();
		int currentIndex = ui->listeWidget_layersList3D->row(item);

		if (item && currentIndex > 0) {

			// Change l'ordre dans la liste des couches
			int currentId = item->data(Qt::UserRole).toInt();

			QWidget *itemWidget = ui->listeWidget_layersList3D->itemWidget(item);
			QWidget *tempWidget = new QWidget();
			QLayout *widgetLayout = itemWidget->layout();
			tempWidget->setLayout(widgetLayout);

			layerList3D->getLayer3D(currentId)->setLayerWidget(tempWidget);

			QListWidgetItem *currentItem = ui->listeWidget_layersList3D->takeItem(currentIndex);

			ui->listeWidget_layersList3D->insertItem(currentIndex-1, currentItem);
			ui->listeWidget_layersList3D->setItemWidget(currentItem, tempWidget);
			ui->listeWidget_layersList3D->setCurrentRow(currentIndex-1);
		}
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


		// Clear Attribute table
		ui->tableWidget_layerAttributeInformation2D->clear();
		ui->tableWidget_layerAttributeInformation2D->setRowCount(0);

		QStringList nameCol;
		nameCol << "Nom" << "Valeur";
		ui->tableWidget_layerAttributeInformation2D->setHorizontalHeaderLabels(nameCol);
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
		}
	}
}

void MainWindow::getAttributesLayer(QMouseEvent *event){
	if (ui->listeWidget_layersList2D->currentItem())
	{
		QListWidgetItem *item = ui->listeWidget_layersList2D->currentItem();
		int currentId = item->data(Qt::UserRole).toInt();

		ui->tableWidget_layerAttributeInformation2D->clear();
		ui->tableWidget_layerAttributeInformation2D->setRowCount(0);

		QStringList nameCol;
		nameCol << "Nom" << "Valeur";
		ui->tableWidget_layerAttributeInformation2D->setHorizontalHeaderLabels(nameCol);

        if((currentId>=1000 && currentId < 2000 && layerList[currentId]->isLayerVisible())||(currentId>=3000 && currentId < 4000 && layerList[currentId]->isLayerVisible()))
		{
			//Get selected shapefile
			Shapefile * shp = ShpList[currentId];
			QPointF mousePos = ui->graphicsView_window2D->mapToScene(event->pos());

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
				for (pqxx::result::const_iterator row = rows_shape.begin(); row != rows_shape.end(); ++row)
				{
					for (unsigned int j = 0; j < row.size(); ++j)
					{
						std::string name_col = rows_shape.column_name(j);
						if (!row[j].is_null())
						{
							std::string value = row[j].as<std::string>();

							// Ajoute une nouvelle ligne pour chaque colonne
							ui->tableWidget_layerAttributeInformation2D->insertRow(j);

							// Remplit la première colonne avec le nom de la colonne
							QTableWidgetItem *col_name_item = new QTableWidgetItem(QString::fromStdString(name_col));
							ui->tableWidget_layerAttributeInformation2D->setItem(j, 0, col_name_item);

							// Remplit la deuxième colonne avec la valeur
							QTableWidgetItem *value_item = new QTableWidgetItem(QString::fromStdString(value));
							ui->tableWidget_layerAttributeInformation2D->setItem(j, 1, value_item);
						}
						else
						{
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
			}
			else
			{
				std::cerr << "Aucune ligne trouvée." << std::endl;
			}
		QMainWindow::mousePressEvent(event);
		}
	}
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui->graphicsView_window2D && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		this->getAttributesLayer(mouseEvent);
		return true;
	}

	return QObject::eventFilter(obj, event);
}

void MainWindow::testAdd3DModel()
{
	//this->storage3D = new ExempleObject3DStorage();
	this->storage3D = new Batiments(ipAdress);
	this->layerList3D->addLayer3DtoOpenGLWidgetAndListWidget(this->storage3D);

}
