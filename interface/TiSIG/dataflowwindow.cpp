#include "dataflowwindow.h"
#include "ui_dataflowwindow.h"

DataFlowWindow::DataFlowWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataFlowWindow)
{
    ui->setupUi(this);
}

DataFlowWindow::~DataFlowWindow()
{
    delete ui;
}
