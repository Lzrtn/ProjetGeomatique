#ifndef WMSFLOW_H
#define WMSFLOW_H

#include <string>
#include <curl/curl.h>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;

class WMSFlow {
public:
    WMSFlow(int crs, string  str_north, string str_south, string str_east, string str_west,
        int width, int height);
    ~WMSFlow();
    void setURL(const char* newurl);
    std::string getUrl();
    void getImage();
    std::string getPath();
private:
    const char* url;
    std::string part_url;
};

#endif // WMSFLOW_H
