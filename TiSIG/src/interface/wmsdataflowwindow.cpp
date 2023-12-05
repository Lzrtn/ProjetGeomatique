#include "wmsdataflowwindow.h"
#include "ui_wmsdataflowwindow.h"
#include <iostream>

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
    QString URL = ui->lineEdit_dataFlowWindow->text();
    url = URL.toStdString();
    accept();
}

void WMSDataFlowWindow::OnButtonValidateDataFlowPreSavedlClicked()
{
    QString flow = ui->comboBox_dataFlowWindow->currentText();

    flowName = flow.toStdString();
    std::string url = "0";
    if(flow == "BDOrtho - WMS"){
        url = "URL vers BDOrtho - WMS";


        //Default extent in Lambert93 and size
//        double xMin = 837354.3000;
//        double yMin = -6520755.6000;
//        double xMax = 842550.9000;
//        double yMax = -6517267.8723;

//        double width = 618;
//        double height = 448;

        std::cout<<"test "<<getViewProjExtent().topLeft().x()<<std::endl;
        double xMin = viewProjExtent.topLeft().x();
        double yMin = viewProjExtent.topLeft().y();
        double xMax = viewProjExtent.bottomRight().x();
        double yMax = viewProjExtent.bottomRight().y();

        double width = viewPxSize.width();
        double height = viewPxSize.height();

        std::cout<<xMin<<" "<<yMin<<" "<<xMax<<" "<<yMax<<std::endl;
        std::cout<<width<<" "<<height<<std::endl;

        /*------------Axel Back Treatments--------------/
         *
         *
         *
         *
         *
         *
         *
        /---------------------------------------------*/

//        filePath = "chemin de l'image stockee en sortie";
        filePath = "../../../ProjetGeomatique/TiSIG/src/data/DONNEES_BDORTHO/Lyon5e_2m.png"; //Chemin TEST
//        filePath = "/home/formation/CGuerrini/Projet_Geomatique/DevClone_flux/ProjetGeomatique/TiSIG/src/data/DONNEES_BDORTHO/Lyon5e_2m.tif";
        accept();
    }

    if(flow == "BDOrtho - WMTS"){
        url = "URL vers BDOrtho - WMTS";
    }

}
