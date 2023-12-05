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
    std::string strURLLowerCase = strURL;
    // Trouver la position de "typename="
    std::transform(strURLLowerCase.begin(), strURLLowerCase.end(), strURLLowerCase.begin(), ::tolower);
    size_t pos = strURLLowerCase.find("typename=");
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
        std::cerr << "typename non trouvé dans l'URL." << std::endl;
        resultDownload = 1;
    }

    // --- Définir filePath ---
    filePath = "data/fluxVect/" + QString::fromStdString(namelayer) + ".zip";
}

int FluxVector::downloadZIP() {

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
            // Si pas d'erreur, creation d'un fichier et ecriture du contenu telecharge
            QFile file(filePath);
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


int FluxVector::unzipFile() {

    QuaZip zip(filePath);
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
            QString extractedFilePath = "data/fluxVect/" + QString::fromStdString(namelayer) + extension;  // Chemin de destination où enregistrer le fichier

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

