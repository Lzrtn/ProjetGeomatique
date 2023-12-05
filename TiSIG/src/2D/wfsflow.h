#ifndef FLUXVECTOR_H
#define FLUXVECTOR_H

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>


class FluxVector
{
public:
    FluxVector(const QString &url);
    int downloadZIP(); // fonction de téléchargement
    int unzipFile(); // fonction de décompression
    int resultDownload = 0;

private:
    QString url;
    std::string namelayer;
    QString filePath;

};

#endif // FLUXVECTOR_H


