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
#include "view_zoom.h"

#include "../src/2D/layer.h"
#include "../src/2D/transformation.h"
#include "../src/2D/shapefile.h"
#include "../src/2D/geotiff.h"
#include "../src/2D/rasteritem.h"
#include "../src/2D/rasterimport.h"
#include "../src/outils/dbmanager.h"
#include "../src/outils/docker.h"

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


	/*_______________________________Initialisation_________________________________________________________________________________________________*/

	ui->setupUi(this);

	// Initialisation in mode 2D //
	ui->stackedWidget->setCurrentIndex(mode);
		// Action 2D
	ui->action_add2DVectorLayer->setEnabled(!mode);
	ui->action_add2DRastorLayer->setEnabled(!mode);
	ui->action_add2DDataFlow->setEnabled(!mode);
		// Actions 3D
	ui->action_add3DVectorLayer->setEnabled(mode);
	ui->action_add3DRastorLayer->setEnabled(mode);
	ui->action_add3DModel->setEnabled(mode);

	ui->openGLWidget_window3D->setCamInfoDisplayer(this);


	/*_______________________________Variables_________________________________________________________________________________________________*/

	// Creating scene for QGraphicsView
	scene = new QGraphicsScene(this);

	// Connect scene to QGraphicsView
	ui->graphicsView_window2D->setScene(scene);
	ui->graphicsView_window2D->setDragMode(QGraphicsView::ScrollHandDrag);
	View_zoom* z = new View_zoom(ui->graphicsView_window2D);
	z->set_modifiers(Qt::NoModifier);

	// ip Address
	ipAdress = ipAdress_d;


	/*_______________________________Barre Gestion Couches et Aide_________________________________________________________________________________________________*/


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
	connect(ui->btn_moveLayerUp, &QPushButton::clicked, [=]() {
		moveItemUp();
	});

	// Connecter le bouton "Down" à la fonction de déplacement vers le bas
	connect(ui->btn_moveLayerDown, &QPushButton::clicked, [=]() {
		moveItemDown();
	});

}

MainWindow::~MainWindow()
{
	// Delete all layers
	for(auto pair: layerList)
	{
		delete pair.second;
		layerList.erase(pair.first);
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
	for (Shapefile* shp : ShpList){
		shp->~Shapefile();
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

	ui->action_add2DVectorLayer->setEnabled(!mode);
	ui->action_add2DRastorLayer->setEnabled(!mode);
	ui->action_add2DDataFlow->setEnabled(!mode);

	ui->action_add3DVectorLayer->setEnabled(mode);
	ui->action_add3DRastorLayer->setEnabled(mode);
	ui->action_add3DModel->setEnabled(mode);

	// when hidden, camera controls are disabled
	if (!mode)
		ui->openGLWidget_window3D->hide();
	else
		ui->openGLWidget_window3D->show();

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
	if (path != ""){

		this->AddShpFileClicked(path);
	}
	return path;

}

std::string MainWindow::OnActionRastorLayerClicked()
{

	QString fileNameRastorLayer = QFileDialog::getOpenFileName(this, tr("Ouvrir une couche de données raster"), "../../../", tr("GeoTIFF (*.tif *.TIF *.tiff)"));
	std::string path = fileNameRastorLayer.toStdString();
	this->AddGeotiffFileClicked(path);
	return path;

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

	//affichage des shapefiles importé
	test.Request("SELECT ST_AsGeoJSON(geom) FROM "+essai1->getTableName()+";");
	pqxx::result rowbis =test.getResult();
	QGraphicsItemGroup *layerGroup = essai1->plotShapefile(rowbis,scene, myColor);
	ui->lineEdit_epsg2D->setText(essai1->getEPSGtoSet());

	layerList[index] = new Layer("Layer "+QString::number(index)+ " : "+ QString(essai1->getTableName().c_str()), true, layerGroup);
	addLayerToListWidget(index, *layerList[index]);
	index++;

	ShpList.push_back(essai1);


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

	QString filename = QString::fromStdString(path);
	RasterItem* rasterItem = RasterImport::CreateRasterItemFromDb(filename,test);

	QGraphicsItemGroup *layerGroup = new QGraphicsItemGroup();
	scene->addItem(layerGroup);


	layerGroup->addToGroup(rasterItem);

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


	// Initie le zIndex de la couche
	layer.setZIndex(index);

	// Créez un nouvel élément pour la couche
	layer.layerItem = new QListWidgetItem(ui->listeWidget_layersList);
	layer.layerItem->setData(Qt::UserRole, index);

	// Créez un widget personnalisé pour cet élément (contenant un label et une case à cocher)
	layer.layerWidget = new QWidget();
	layer.layout = new QHBoxLayout(layer.layerWidget);


	layer.visibilityCheckbox = new QCheckBox("");
	layer.visibilityCheckbox->setChecked(layer.isLayerVisible());
	layer.layerLabel = new QLabel(layer.getLayerName());

	// Connectez le signal clicked de la case à cocher à une fonction pour gérer la visibilité
	connect(layer.visibilityCheckbox, &QCheckBox::toggled, [=](bool checked) {
		layerList[index]->getLayerGroup()->setVisible(checked);
		layerList[index]->setLayerVisible(checked);
	});


	layer.layout->addWidget(layer.visibilityCheckbox);
	layer.layout->addWidget(layer.layerLabel);
	layer.layout->setAlignment(Qt::AlignLeft);

	layer.layout->setContentsMargins(2, 0, 2, 0); // Taille de la ligne de la couche dans le gestionnaire
	layer.layout->setSpacing(10); // Écart entre la checkbox et le nom de la couche

	layer.layerWidget->setLayout(layer.layout);
	layer.layerItem->setSizeHint(layer.layerWidget->sizeHint());



	ui->listeWidget_layersList->setItemWidget(layer.layerItem, layer.layerWidget);

	// met à jour l'ordre de superpositions des couches
	updateLayerOrderInGraphicsView();
}

void MainWindow::moveItemDown() {
	QListWidgetItem *item = ui->listeWidget_layersList->currentItem();
	int currentIndex = ui->listeWidget_layersList->row(item);

	if (item && currentIndex < ui->listeWidget_layersList->count()-1)
	{
		// Change la profondeur des couches
		int currentId = item->data(Qt::UserRole).toInt();
		QListWidgetItem *nextItem = ui->listeWidget_layersList->item(currentIndex+1);
		int nextId = nextItem->data(Qt::UserRole).toInt();

		int currentZIndex = layerList[currentId]->getZIndex();
		int prevZIndex = layerList[nextId]->getZIndex();

		layerList[currentId]->setZIndex(prevZIndex);
		layerList[nextId]->setZIndex(currentZIndex);

		updateLayerOrderInGraphicsView();

        // Change l'ordre dans la liste des couches
        QWidget *itemWidget = ui->listeWidget_layersList->itemWidget(item);
        QWidget *tempWidget = new QWidget();
        QLayout *widgetLayout = itemWidget->layout();
        tempWidget->setLayout(widgetLayout);

        layerList[currentId]->layerWidget = tempWidget;

		QListWidgetItem *currentItem = ui->listeWidget_layersList->takeItem(currentIndex);

		ui->listeWidget_layersList->insertItem(currentIndex+1, currentItem);
		ui->listeWidget_layersList->setItemWidget(currentItem, tempWidget);
		ui->listeWidget_layersList->setCurrentRow(currentIndex+1);
	}
}

void MainWindow::moveItemUp() {
	QListWidgetItem *item = ui->listeWidget_layersList->currentItem();
	int currentIndex = ui->listeWidget_layersList->row(item);

	if (item && currentIndex > 0) {

		// Change la profondeur des couches
		int currentId = item->data(Qt::UserRole).toInt();
		QListWidgetItem *prevItem = ui->listeWidget_layersList->item(currentIndex-1);
		int prevId = prevItem->data(Qt::UserRole).toInt();

		int currentZIndex = layerList[currentId]->getZIndex();
		int prevZIndex = layerList[prevId]->getZIndex();

		layerList[currentId]->setZIndex(prevZIndex);
		layerList[prevId]->setZIndex(currentZIndex);

		updateLayerOrderInGraphicsView();

        // Change l'ordre dans la liste des couches
        QWidget *itemWidget = ui->listeWidget_layersList->itemWidget(item);
        QWidget *tempWidget = new QWidget();
        QLayout *widgetLayout = itemWidget->layout();
        tempWidget->setLayout(widgetLayout);

        layerList[currentId]->layerWidget = tempWidget;

		QListWidgetItem *currentItem = ui->listeWidget_layersList->takeItem(currentIndex);

		ui->listeWidget_layersList->insertItem(currentIndex-1, currentItem);
		ui->listeWidget_layersList->setItemWidget(currentItem, tempWidget);
		ui->listeWidget_layersList->setCurrentRow(currentIndex-1);
	}
}

void MainWindow::updateLayerOrderInGraphicsView() {
	for(auto pair: layerList)
	{
		pair.second->getLayerGroup()->setZValue(pair.second->getZIndex());
	}
	ui->graphicsView_window2D->repaint();
}
