#include "wmsdataflowwindow.h"
#include "ui_wmsdataflowwindow.h"


WMSDataFlowWindow::WMSDataFlowWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WMSDataFlowWindow)
{
    ui->setupUi(this);
    // Initialisation de la ComboBox //
        ui->comboBox_dataFlowWindow->addItems({"BDOrtho - WMS", "BDOrtho - WMTS"});

        // Connecting Validate DataFlowUrl button
        connect(ui->btn_validateDataFlowUrl, &QPushButton::clicked, this, &WMSDataFlowWindow::OnButtonValidateDataFlowUrlClicked);
        //connect(ui->btn_validateDataFlowUrl, SIGNAL(clicked()), this, SLOT(OnButtonValidateDataFlowUrlClicked()));
        // Connecting Validate DataFlowPreSaved button
        connect(ui->btn_validateDataFlowPreSaved, &QPushButton::clicked, this, &WMSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked);
        //connect(ui->btn_validateDataFlowPreSaved, SIGNAL(clicked()), this, SLOT(OnButtonValidateDataFlowPreSavedlClicked()));
}

WMSDataFlowWindow::~WMSDataFlowWindow()
{
    delete ui;
}


void WMSDataFlowWindow::OnButtonValidateDataFlowUrlClicked()
{
    url = ui->lineEdit_dataFlowWindow->text().toStdString();
    accept();
}


void WMSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();
    flowName = flow.toStdString();

    if(flow == "BDOrtho - WMS"){

//        url = "URL vers BDOrtho - WMS";
        url = "https://data.geopf.fr/wms-r?LAYERS=OI.OrthoimageCoverage&EXCEPTIONS=text/xml&FORMAT=image/jpeg&SERVICE=WMS&VERSION=1.3.0&REQUEST=GetMap&STYLES=";
    }
    if(flow == "BDOrtho - WMTS"){
        url = "URL vers BDOrtho - WMTS";
    }
    accept();
}
