#include "dataflowwindow.h"
#include "ui_dataflowwindow.h"

DataFlowWindow::DataFlowWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataFlowWindow)
{
    ui->setupUi(this);

    // Initialisation de la ComboBox //
    ui->comboBox_dataFlowWindow->addItems({"BDTopo - Bâti", "BDTopo - Route", "BDOrtho"});
}

DataFlowWindow::~DataFlowWindow()
{
    delete ui;
}
