// wms.h
#ifndef WMS_H
#define WMS_H
#include <string>
#include <opencv2/opencv.hpp>

class WMS {
public:
    WMS(int crs, double north_limit, double south_limit, double east_limit, double west_limit,
        int width, int height);
    ~WMS();
    const char* getUrl();
private:
    const char* url;

};

#endif // WMS_H