#include "fluxvector.h"

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include <string>
#include <iostream>



FluxVector::FluxVector(const QString &url_)
    :url(url_)
{
    // --- Définir namelayer ---
    std::string strURL = url_.toStdString();
    // Trouver la position de "typename="
    size_t pos = strURL.find("typename=");
    if (pos != std::string::npos) {
        // Extraire la sous-chaîne après "typename="
        namelayer = strURL.substr(pos + sizeof("typename=") - 1);
        // Trouver la position de l'ampersand ou de la fin de la chaîne
        size_t endPos = namelayer.find("&");
        if (endPos != std::string::npos) {
            namelayer = namelayer.substr(0, endPos);
        }
        std::cout << "namelayer: " << namelayer << std::endl;
    } else {
        std::cerr << "typename= non trouvé dans l'URL." << std::endl;
    }

    // --- Définir filePath ---
    filePath = "data/fluxVect/"+namelayer+".zip";

}

void FluxVector::downloadZIP() {
    // Creation d'un gestionnaire reseau
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // Configuration de la requete reseau (specification de l'URL du fichier ZIP a telecharger
    QNetworkRequest request;
    request.setUrl(QUrl(url));

    // Emission de la requete de telechargement
    QNetworkReply *reply = manager->get(request);

    // Connection au signal "finished" du telechargement : lorsque le telechargement est termine, fonction de traitement de fin de telechargement
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            std::cout<<"Telechargement effectue avec succes."<<std::endl;
            // Si pas d'erreur, creation d'un fichier et ecriture du contenu telecharge
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
            }
        } else {
            // Erreur lors du téléchargement
            std::cerr<<"Erreur lors du téléchargement : "<<reply->errorString().toStdString()<<std::endl;
        }
        reply->deleteLater();
    });
}


void FluxVector::unzipFile() {
    QuaZip zip(filePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        std::cerr<<"Erreur lors de l'ouverture du fichier zip."<<std::endl;
        return;
    }

    QuaZipFile file(&zip);
    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        if (!zip.getCurrentFileName().isEmpty()) {
            file.open(QIODevice::ReadOnly);
            QString fileName = zip.getCurrentFileName();
            // Traitez le fichier extrait ici
            // Par exemple, enregistrez-le sur le disque
            file.close();
        }
    }
    zip.close();
    std::cout<<"Fichier dézippé avec succès."<<std::endl;
}




