// wms.h
#ifndef WMS_H
#define WMS_H
#include <string>
#include <curl/curl.h>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;

class WMS {
public:
    WMS(int crs, string  str_north, string str_south, string str_east, string str_west,
        int width, int height);
    ~WMS();
    void setURL(const char* newurl);
    const char* getUrl();
    void getImage();
    std::string getPath();
private:
    const char* url;
};

#endif // WMS_H