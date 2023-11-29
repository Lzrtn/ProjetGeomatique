#include "dataflowwindow.h"
#include "ui_dataflowwindow.h"
#include "../src/2D/fluxvector.h"

#include <iostream>
#include <string>
using namespace std;
#include <QDebug>
#include <algorithm>

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
    connect(ui->btn_validateDataFlowPreSaved, &QPushButton::clicked, this, &DataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked);
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

const char* DataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();
    const char* lien = "0";
    if(flow == "BDTopo - Bâti"){
        lien = "URL vers BDTopo Bati";
    }
    if(flow == "BDTopo - Route"){
        lien = "https://data.geopf.fr/wfs/ows?VERSION=2.0.0&OUTPUTFORMAT=SHAPE-ZIP&request=GetFeature&service=wfs&bbox=45.727093,4.819074,45.746508,4.850961&srsname=EPSG:2154&typename=BDTOPO_V3:troncon_de_route";
    }
    if(flow == "BDOrtho"){
        lien = "URL vers BDOrtho";
    }
    FluxVector *fluxVect = new FluxVector(lien);
    fluxVect->downloadZIP();
    fluxVect->unzipFile();

    return lien;
}

