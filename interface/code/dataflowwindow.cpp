#include "dataflowwindow.h"
#include "ui_dataflowwindow.h"
#include <iostream>
#include <string>
#include <QDebug>

DataFlowWindow::DataFlowWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataFlowWindow)
{
    ui->setupUi(this);

    // Initialisation de la ComboBox //
    ui->comboBox_dataFlowWindow->addItems({"BDTopo - Bâti", "BDTopo - Route", "BDOrtho"});

    connect(ui->btn_validateDataFlowUrl, &QPushButton::clicked, this, &DataFlowWindow::OnButtonValidateDataFlowUrlClicked);
}

DataFlowWindow::~DataFlowWindow()
{
    delete ui;
}

const char* DataFlowWindow::OnButtonValidateDataFlowUrlClicked()
{
    QString url = ui->lineEdit_dataFlowWindow->text();
    return url.toLocal8Bit().constData();
}

