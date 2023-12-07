#ifndef WMTSFLOW_H
#define WMTSFLOW_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#include <fstream>

using namespace std;

class WMTSFlow {
public:
    WMTSFlow(int tilematrix, double north_limit, double west_limit, int width, int height);
    ~WMTSFlow();
    void setURL(const char* newurl);
    vector<vector<const char*>> getUrl();
    void getImage();
    void combine();
private:
    const char* url;
    vector<vector<const char*>> tablurl;
};

#endif // WMTS_H
