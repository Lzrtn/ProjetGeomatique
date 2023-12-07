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
    if(flow == "BDOrtho - WMS"){
        url = "URL vers BDOrtho - WMS";
    }
    if(flow == "BDOrtho - WMTS"){
        url = "URL vers BDOrtho - WMTS";
    }
    accept();
}
