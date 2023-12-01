#include "WFSdataflowwindow.h"
#include "ui_WFSdataflowwindow.h"
#include <iostream>
#include <string>
#include <QDebug>

WFSDataFlowWindow::WFSDataFlowWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WFSDataFlowWindow)
{
    ui->setupUi(this);

    // Initialisation de la ComboBox //
    ui->comboBox_dataFlowWindow->addItems({"BDTopo - Bâti", "BDTopo - Route"});

    // Connecting Validate DataFlowUrl button
    connect(ui->btn_validateDataFlowUrl, &QPushButton::clicked, this, &WFSDataFlowWindow::OnButtonValidateDataFlowUrlClicked);

    // Connecting Validate DataFlowPreSaved button
    connect(ui->btn_validateDataFlowPreSaved, &QPushButton::clicked, this, &WFSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked);
}

WFSDataFlowWindow::~WFSDataFlowWindow()
{
    delete ui;
}

const char* WFSDataFlowWindow::OnButtonValidateDataFlowUrlClicked()
{
    QString url = ui->lineEdit_dataFlowWindow->text();
    return url.toLocal8Bit().constData();
}

const char* WFSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();
    const char* lien = "0";
    if(flow == "BDTopo - Bâti"){
        lien = "URL vers BDTopo Bati";
    }
    if(flow == "BDTopo - Route"){
        lien = "URL vers BDTopo Route";
    }
    if(flow == "BDOrtho"){
        lien = "URL vers BDOrtho";
    }
    return lien;
}

