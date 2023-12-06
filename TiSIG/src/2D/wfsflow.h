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
    WFSFlow(const std::string &url, const std::string &longmin, const std::string &latmin, const std::string &longmax, const std::string &latmax);
    int downloadZIP(); // fonction de téléchargement
    int unzipFile(); // fonction de décompression
    int resultDownload = 0;
    std::string GetfilePath(){ return filePath; };

private:
    std::string url;
    std::string namelayer;
    std::string filePath;

    std::string longmin;
    std::string latmin;
    std::string longmax;
    std::string latmax;

};

#endif // WFSFLOW_H


