// wms.h
#ifndef WMS_H
#define WMS_H
#include <string>
#include <curl/curl.h>
#include <opencv2/opencv.hpp>
#include <fstream>

class WMS {
public:
    WMS(int crs, double north_limit, double south_limit, double east_limit, double west_limit,
        int width, int height);
    ~WMS();
    const char* getUrl();
    void getImage();
    std::string getPath();
private:
    const char* url;
};

#endif // WMS_H