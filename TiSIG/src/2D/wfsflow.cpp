#include "wfsflow.h"

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include <string>
#include <iostream>



WFSFlow::WFSFlow(const std::string &url_, const std::string &longmin_, const std::string &latmin_, const std::string &longmax_, const std::string &latmax_)
    :url(url_), longmin(longmin_), latmin(latmin_), longmax(longmax_), latmax(latmax_)
{
    // --- Définir namelayer --- en trouvant la position de "typename="
    url = url_+"&REQUEST=GetFeature&OUTPUTFORMAT=SHAPE-ZIP&BBOX="+longmin+","+latmin+","+longmax+','+latmax+"&SRSNAME=EPSG:2154";
    std::string strURLLowerCase = url;
    std::transform(strURLLowerCase.begin(), strURLLowerCase.end(), strURLLowerCase.begin(), ::tolower);
        size_t pos = strURLLowerCase.find("typename=");
        if (pos != std::string::npos) {
            // Extraire la sous-chaîne après "typename="
            namelayer = url.substr(pos + sizeof("typename=") - 1);
            // Trouver la position de l'ampersand ou de la fin de la chaîne
            size_t endPos = namelayer.find("&");
            if (endPos != std::string::npos) {
                namelayer = namelayer.substr(0, endPos);
                std::replace(namelayer.begin(), namelayer.end(), ':', '_');
            }
    } else {
        std::cerr << "typename non trouvé dans l'URL." << std::endl;
        resultDownload = 1;
    }

    // --- Définir filePath ---
    filePath = "data/wfsFlow/" + namelayer + ".zip";

    std::cout << "URL de cette couche : " << url << std::endl;
    std::cout << "Nom de cette couche : " << namelayer<<std::endl;
    std::cout << "Chemin de cette couche : " << filePath<<std::endl;
}

WFSFlow::~WFSFlow() {
    std::cout << "destrcuteur WFSFlow" << std::endl;
    if (shpfile) delete shpfile;
}

int WFSFlow::downloadZIP() {

    // Creation d'un gestionnaire reseau
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // Configuration de la requete reseau (specification de l'URL du fichier ZIP a telecharger
    QNetworkRequest request;
    request.setUrl(QUrl(QString::fromStdString(url)));

    // Emission de la requete de telechargement
    QNetworkReply *reply = manager->get(request);

    // Connection au signal "finished" du telechargement : lorsque le telechargement est termine, fonction de traitement de fin de telechargement
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Si pas d'erreur, creation d'un fichier et ecriture du contenu telecharge
            QFile file(QString::fromStdString(filePath));
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
            }
            std::cout<<"Telechargement effectue avec succes."<<std::endl;
        } else {
            // Erreur lors du téléchargement
            std::cerr<<"Erreur lors du téléchargement : "<<reply->errorString().toStdString()<<std::endl;
            resultDownload = 1;
        }
        reply->deleteLater(); // destrcution de l'objet QNetworkReply

        // Appel de la fonction unzipFile() une fois le téléchargement terminé avec succès
        if (resultDownload == 0) {
            unzipFile();
        }
    });

    return resultDownload;
}


int WFSFlow::unzipFile() {

    QuaZip zip(QString::fromStdString(filePath));
    if (!zip.open(QuaZip::mdUnzip)) {
        std::cerr << "Erreur lors de l'ouverture du fichier zip." << std::endl;
        resultDownload = 1;
        return resultDownload;
    }

    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        if (!zip.getCurrentFileName().isEmpty()) {
            QuaZipFile file(&zip);
            if (!file.open(QIODevice::ReadOnly)) {
                std::cerr << "Erreur lors de l'ouverture du fichier dans l'archive." << std::endl;
                resultDownload = 1;
                return resultDownload;
            }

            QString fileName = zip.getCurrentFileName();
            QString extension = fileName.right(4);
            // Traitez le fichier extrait ici
            QString extractedFilePath = "data/wfsFlow/" + QString::fromStdString(namelayer) + extension;  // Chemin de destination où enregistrer le fichier

            QFile extractedFile(extractedFilePath);
            if (!extractedFile.open(QIODevice::WriteOnly)) {
                std::cerr << "Erreur lors de l'ouverture du fichier extrait pour écriture." << std::endl;
                resultDownload = 1;
                file.close();
                return resultDownload;
            }

            extractedFile.write(file.readAll());
            extractedFile.close();
            file.close();
        }
    }
    zip.close();
    std::cout << "Fichiers dézippés avec succès." << std::endl;
    return resultDownload;
}

