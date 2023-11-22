#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"
#include "dataflowwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::OnButtonSwitchTo2D3DClicked()
{
    mode = !mode;
    ui->stackedWidget->setCurrentIndex(mode);

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
    QString fileNameVectorLayer = QFileDialog::getOpenFileName(this, tr("Ouvrir une couche de données vecteur"), "/home", tr("ShapeFile (*.shp)"));
    std::cout<<fileNameVectorLayer.toStdString()<<std::endl;
    return fileNameVectorLayer.toStdString();
}

std::string MainWindow::OnActionRastorLayerClicked()
{
    QString fileNameRastorLayer = QFileDialog::getOpenFileName(this, tr("Ouvrir une couche de données raster"), "/home", tr("GeoTIFF (*.tif)"));
    std::cout<<fileNameRastorLayer.toStdString()<<std::endl;
    return fileNameRastorLayer.toStdString();
}

std::string MainWindow::OnAction3DModelClicked()
{
    QString fileName3Dmodel = QFileDialog::getOpenFileName(this, tr("Ouvrir un modèle 3D"), "/home", tr("Modèle 3D (*.gml *.asc)"));
    std::cout<<fileName3Dmodel.toStdString()<<std::endl;
    return fileName3Dmodel.toStdString();
}

