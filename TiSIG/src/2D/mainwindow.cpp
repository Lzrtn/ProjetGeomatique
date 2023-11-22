#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"
#include "dataflowwindow.h"
#include <iostream>

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

    // Connecting addd2ddataflow action
    connect(ui->action_add2DDataFlow, &QAction::triggered, this, &MainWindow::OnAction2DDataFlowClicked);
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
