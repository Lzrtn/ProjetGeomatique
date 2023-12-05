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

    // Connecting Validate DataFlowUrl button
    connect(ui->btn_validateDataFlowUrl, &QPushButton::clicked, this, &DataFlowWindow::OnButtonValidateDataFlowUrlClicked);

    // Connecting Validate DataFlowPreSaved button
//    connect(ui->btn_validateDataFlowPreSaved, &QPushButton::clicked, this, &DataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked);

    connect(ui->btn_validateDataFlowPreSaved, SIGNAL(clicked()), this, SLOT(OnButtonValidateDataFlowPreSavedlClicked()));
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

void DataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();
//    const char* lien = "0";
    if(flow == "BDTopo - Bâti"){
        lien = "URL vers BDTopo Bati";
    }
    if(flow == "BDTopo - Route"){
        lien = "URL vers BDTopo Route";
    }
    if(flow == "BDOrtho"){
        lien = "URL vers BDOrtho";
    }
    accept();
}

