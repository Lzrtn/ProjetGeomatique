// wmts.h
#ifndef WMTS_H
#define WMTS_H
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;

class WMTS {
public:
    WMTS(string tilematrixset, int tilematrix, 
        double north_limit, double west_limit);
    ~WMTS();
    const char* getUrl();
private:
    const char* url;

};

#endif // WMTS_H