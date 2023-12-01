#include <iostream>
#include <fstream>
#include <curl/curl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include "wms.h"
#include "wmts.h"

using namespace std;

int main() {
    //Initialize
    WMS wmsInstance(4326, 45.98624572753906, 45.98514572753706, 4.98760461807251, 4.98650461807051, 1080, 1080);
    //WMTS wmtsInstance( 14, 536227.222852, 5739265.686868,1080,1080);

    //Getting Url
    const char* url = wmsInstance.getUrl();
    std::cout<<url;
    //vector<vector<const char*>> url2 = wmtsInstance.getUrl();

    /*//Getting Image
    wmsInstance.getImage();
    wmtsInstance.getImage();
    wmtsInstance.combine();*/

    return 0;
}