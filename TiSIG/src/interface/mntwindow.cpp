#include "mntwindow.h"
#include "ui_mntwindow.h"
#include "../2D/geotiff_to_obj.h"

#include <iostream>
#include <QFileDialog>



MntWindow::MntWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MntWindow)
{
    ui->setupUi(this);

    // Connecting Validate DataFlowUrl button
    connect(ui->btn_DTMFile, &QPushButton::clicked, this, &MntWindow::OnButtonAddDTMClicked);


    // Connecting Validate DataFlowUrl button
    connect(ui->btn_OrthoFile, &QPushButton::clicked, this, &MntWindow::OnButtonAddOrthoClicked);

    // Connecting Validate DataFlowUrl button
//    connect(ui->btn_Submit, &QPushButton::clicked, this, &MntWindow::OnButtonSubmitClicked);

    // Connecte le slot du QDialog au signal du bouton du QDialog
    connect(ui->btn_Submit, SIGNAL(clicked()), this, SLOT(OnButtonSubmitClicked()));
}

MntWindow::~MntWindow()
{
    delete ui;
}


void MntWindow::OnButtonAddDTMClicked() {

    QString fileNameDTM = QFileDialog::getOpenFileName(this, tr("Ouvrir un MNT"), "../../../", tr("GeoTIFF (*.tif *.TIF *.tiff)"));
    std::string path = fileNameDTM.toStdString();
    ui->lineEdit_DTMPath->setText(fileNameDTM);
//    std::cout<< path <<std::endl;
}


void MntWindow::OnButtonAddOrthoClicked() {
    QString fileNameOrtho = QFileDialog::getOpenFileName(this, tr("Ouvrir une Orthoimage"), "../../../", tr("GeoTIFF (*.tif *.TIF *.tiff)"));
    std::string path = fileNameOrtho.toStdString();
    ui->lineEdit_OrthoPath->setText(fileNameOrtho);
//    std::cout<< path <<std::endl;
}


void MntWindow::OnButtonSubmitClicked() {
    QString fileNameDTM = ui->lineEdit_DTMPath->text();
    QByteArray ba = fileNameDTM.toLocal8Bit();
    const char *pathDTM = ba.data();
//    const char* pathDTM = fileNameDTM.toLocal8Bit().constData();


    QString fileNameOrtho = ui->lineEdit_OrthoPath->text();
    QByteArray bb = fileNameDTM.toLocal8Bit();
    const char *pathOrtho = bb.data();
//    const char* pathOrtho = fileNameOrtho.toLocal8Bit().constData();

    if(fileNameDTM=="" || fileNameOrtho=="") {
        ui->label_Error->setText("Veuillez renseigner tous les chemins vers les fichiers.");
    } else {
        std::cout << pathDTM << std::endl;
        GeoTiffToObjConverter converter(pathDTM, pathOrtho, "./data/DONNEES_BDORTHO/");

        converter.writeObjFileWithTextures();
        this->value = 5;
//        converter.writeCoordWithTextures();


//        for (auto vertex : converter.getPosition())
//         {
//             std::cout << vertex.x() << ' ' << vertex.y() << ' ' << vertex.z() << std::endl;
//         }
        accept();
    }
}
