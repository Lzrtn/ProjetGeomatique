#include "dataflowwindow.h"
#include "ui_dataflowwindow.h"
#include "../src/2D/fluxvector.h"

#include <iostream>
#include <string>
using namespace std;
#include <QDebug>
#include <algorithm>

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>



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

void DataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();
    std::string url = "0";
    if(flow == "BDTopo - Bâti"){
        url = "https://data.geopf.fr/wfs/ows?VERSION=2.0.0&SERVICE=wfs&TYPENAME=BDTOPO_V3:batiment";
    }
    if(flow == "BDTopo - Route"){
        url = "https://data.geopf.fr/wfs/ows?VERSION=2.0.0&SERVICE=wfs&TYPENAME=BDTOPO_V3:troncon_de_route";
    }
//    if(flow == "BDOrtho"){
//        url = "???";
//    }

    // Emprise de la fenêtre à récupérer
    std::string longmin = std::to_string(45.727093);
    std::replace(longmin.begin(), longmin.end(), ',', '.');
    std::string latmin = std::to_string(4.819074);
    std::replace(latmin.begin(), latmin.end(), ',', '.');
    std::string longmax = std::to_string(45.746508);
    std::replace(longmax.begin(), longmax.end(), ',', '.');
    std::string latmax = std::to_string(4.850961);
    std::replace(latmax.begin(), latmax.end(), ',', '.');

    QString wfsDataSource = QString::fromStdString(url+"&REQUEST=GetFeature&OUTPUTFORMAT=SHAPE-ZIP&BBOX="+longmin+","+latmin+","+longmax+','+latmax+"&SRSNAME=EPSG:2154");
    std::cout<<wfsDataSource.toStdString()<<std::endl;
    FluxVector *fluxVect = new FluxVector(wfsDataSource);
    fluxVect->downloadZIP();

}




