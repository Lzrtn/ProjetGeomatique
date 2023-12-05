#ifndef WFSFLOW_H
#define WFSFLOW_H

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>


class WFSFlow
{
public:
    WFSFlow(const QString &url);
    int downloadZIP(); // fonction de téléchargement
    int unzipFile(); // fonction de décompression
    int resultDownload = 0;
    QString filePath;
private:
    QString url;
    std::string namelayer;


};

#endif // WFSFLOW_H


