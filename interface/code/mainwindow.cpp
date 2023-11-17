#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation en mode 2D
    ui->stackedWidget->setCurrentIndex(mode);
    ui->btn_cameraRotation->setVisible(mode);
    ui->label_attributeInformation->setVisible(!mode);
    ui->tableWidget_layerAttributeInformation->setVisible(!mode);


    // On connecte le bouton switch 2D/3D
    connect(ui->btn_switchMode2D3D, &QPushButton::clicked, this, &MainWindow::OnButtonSwitchTo2D3DClicked);


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

//    ui->label_attributeInformation->update();
//    ui->tableWidget_layerAttributeInformation->update();
}


