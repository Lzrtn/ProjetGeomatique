#include "wfsdataflowwindow.h"
#include "ui_wfsdataflowwindow.h"

#include "../src/2D/wfsflow.h"
#include "../src/interface/mainwindow.h"

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



WFSDataFlowWindow::WFSDataFlowWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WFSDataFlowWindow)
{
    ui->setupUi(this);
    // Initialisation de la ComboBox //
        ui->comboBox_dataFlowWindow->addItems({"BDTopo - Bâti", "BDTopo - Route"});

        // Connecting Validate DataFlowUrl button
        connect(ui->btn_validateDataFlowUrl, &QPushButton::clicked, this, &WFSDataFlowWindow::OnButtonValidateDataFlowUrlClicked);
        //connect(ui->btn_validateDataFlowUrl, SIGNAL(clicked()), this, SLOT(OnButtonValidateDataFlowUrlClicked()));
        // Connecting Validate DataFlowPreSaved button
        connect(ui->btn_validateDataFlowPreSaved, &QPushButton::clicked, this, &WFSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked);
        //connect(ui->btn_validateDataFlowPreSaved, SIGNAL(clicked()), this, SLOT(OnButtonValidateDataFlowPreSavedlClicked()));
}

WFSDataFlowWindow::~WFSDataFlowWindow()
{
    delete ui;
}


void WFSDataFlowWindow::OnButtonValidateDataFlowUrlClicked()
{
    QString url = ui->lineEdit_dataFlowWindow->text();

    FlowProcessing(url.toStdString());

    accept();
}

void WFSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();
    std::string url = "0";
    if(flow == "BDTopo - Bâti"){
        url = "https://data.geopf.fr/wfs/ows?VERSION=2.0.0&SERVICE=wfs&TYPENAME=BDTOPO_V3:batiment";
    }
    if(flow == "BDTopo - Route"){
        url = "https://data.geopf.fr/wfs/ows?VERSION=2.0.0&SERVICE=wfs&TYPENAME=BDTOPO_V3:troncon_de_route";
    }

    FlowProcessing(url);

    accept();
}



void WFSDataFlowWindow::FlowProcessing(std::string url){
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
    WFSFlow *wfsflow = new WFSFlow(wfsDataSource);
    wfsflow->downloadZIP();
    path = wfsflow->filePath.toStdString();
    path.replace(path.size() - 4, 4, ".shp");

//    emit processingFinished(path); // Emis lorsque le traitement est terminé

}



