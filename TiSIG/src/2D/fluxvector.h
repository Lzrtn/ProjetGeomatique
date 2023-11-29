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
    void downloadZIP();
    void unzipFile();
private:
    QString url;
    std::string namelayer;
    QString filePath;
};

#endif // FLUXVECTOR_H

