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
    url = ui->lineEdit_dataFlowWindow->text().toStdString();
    accept();
}

void WFSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();
    if(flow == "BDTopo - Bâti"){
        url = "https://data.geopf.fr/wfs/ows?VERSION=2.0.0&SERVICE=wfs&TYPENAME=BDTOPO_V3:batiment";
    }
    if(flow == "BDTopo - Route"){
        url = "https://data.geopf.fr/wfs/ows?VERSION=2.0.0&SERVICE=wfs&TYPENAME=BDTOPO_V3:troncon_de_route";
    }
    accept();
}






