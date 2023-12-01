#include "wfsdataflowwindow.h"
#include "ui_wfsdataflowwindow.h"

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

std::string WFSDataFlowWindow::getLien() const{
    return lien;
}

void WFSDataFlowWindow::OnButtonValidateDataFlowUrlClicked()
{
    QString url = ui->lineEdit_dataFlowWindow->text();
    lien = url.toStdString();
    accept();
}

void WFSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();
    std::string url = "0";
    if(flow == "BDTopo - Bâti"){
        url = "URL vers BDTopo Bati";
    }
    if(flow == "BDTopo - Route"){
        url = "URL vers BDTopo Route";
    }
    lien = url;
    accept();
}
