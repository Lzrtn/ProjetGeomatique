#include <iostream>
#include <fstream>
#include <curl/curl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <opencv2/opencv.hpp>

using namespace std;

// Callback function to write the received data to a file
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::ostream* out = static_cast<std::ostream*>(userp);
    out->write(static_cast<char*>(contents), realsize);
    return realsize;
}


int main() {

    //Service utils
    string wms = "WMS";
    string wmts = "WMTS";
    string link_wmts = "wmts";
    string link_wms_raster = "wms-r";

    //Import geographical influence from front
    double import_north = 45.88624572753906;
    double import_south = 45.43631057739258;
    double import_east  = 4.98760461807251;
    double import_west  = 4.668561363220215;

    //Import screen size from front
    int front_screen_width  = 1080;
    int front_screen_height = 1080;

    //Import level of zoom
    int import_tilematrix = 18;

    //Convert into string for WMS request
    string str_north = to_string(import_north);
    string str_south = to_string(import_south);
    string str_east  = to_string(import_east);
    string str_west  = to_string(import_west);
    string str_width = to_string(front_screen_width);
    string str_heigh = to_string(front_screen_height);

    //Top Left Corner utils (coordinate in different projections)
    double x0_tlc_lamb93          = 0.0000000000000000;
    double y0_tlc_lamb93          = 12000000.0000000000000000;

    double x0_tlc_lamb93_5cm      = 0.0000000000000000;
    double y0_tlc_lamb93_5cm      = 12000000.0000000000000000;

    double x0_tlc_lamb93_5cm_epsg = 0.0000000000000000;
    double y0_tlc_lamb93_5cm_epsg = 12000000.0000000000000000;

    double x0_tlc_lamb93_1m_mnt   = -500.0000000000000000;
    double y0_tlc_lamb93_1m_mnt   = 12000500.0000000000000000;

    double x0_tlc_ignf_lamb93     = -500.0000000000000000;
    double y0_tlc_ignf_lamb93     = 12000500.0000000000000000;

    double x0_tlc_pm              = -20037508.3427892476320267;
    double y0_tlc_pm              = 20037508.3427892476320267;
    
    double x0_tlc_wgs84g          = -180.0000000000000000;
    double y0_tlc_wgs84g          = 90.0000000000000000;

    double x0_tlc_wgs84g_po       = -180.0000000000000000;
    double y0_tlc_wgs84g_po       = 90.0000000000000000;

    double x0_tlc_pm5cm           = -20037508.3427892476320267;
    double y0_tlc_pm5cm           = 20037508.3427892476320267;    

    //Calculate TLC
    double x_tlc = import_west - x0_tlc_wgs84g;
    double y_tlc = import_north - y0_tlc_wgs84g;
    double distance_per_tile = 256 * ;

    //String utils

    //General : 
    string coma     = ",";
    string key_bbox = "&BBOX=";
    //For WMS-Raster : 
    string key_width  = "&WIDTH=";
    string key_height = "&HEIGHT=";
    //For WMTS :
    string key_tile_matrix_set = "&TILEMATRIXSET=";
    string key_tile_matrix     = "&TILEMATRIX=";
    string key_tile_col        = "&TILECOL=";
    string key_tile_row        = "&TILEROW=";

    // Create string object for WMS query

    //Base url of the flow
    string link       = "https://data.geopf.fr/";
    //LAYERS = the layer you want to show
    string layer      = "?LAYERS=OI.OrthoimageCoverage";
    string exception  = "&EXCEPTIONS=text/xml";
    //FORMAT = format of the image get on the flow (jpeg/png/tiff/geotiff) 
    //(Warning : depending on the flow not all the format could be handled)
    string format     = "&FORMAT=image/png";
    //SERVICE = type of service you want (WMS/WFS/WMTS/TMS) 
    //(Warning : the following keys are only for WMS flows)
    string service    = "&SERVICE=WMS";
    string version    = "&VERSION=1.3.0";
    //REQUEST = type of get you want (GetMap/GetTiles/GetCapabilities)
    //(Warning : the type of get depends of the service you ask for)
    string request    = "&REQUEST=GetMap";
    string styles     = "&STYLES=";
    //CRS = EPSG: projection key (integer)
    string crs        = "&CRS=EPSG:4326";
    //BBOX = south, west, north, east (double)
    string bbox       = key_bbox+str_south+coma+str_west+coma+str_north+coma+str_east;
    //WIDTH,HEIGHT = screen size in pixels (integer)
    string width_request  = key_width+str_width;
    string height_request = key_height+str_heigh;

    // Concatenate using the + operator
    std::string result = link + link_wms_raster + layer + exception + format + service + version +
                         request + styles + crs + bbox + width_request + height_request;

    //Convert into char*
    const char* url = strdup(result.c_str());

    // Initialize libcurl
    CURL* curl = curl_easy_init();

    if (curl) {

        // Set the URL to download
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set up the callback to write the data to a file
        std::ofstream imageFile("downloaded_image.tiff", std::ios::binary);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageFile);

        // Perform the HTTP request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Cleanup
        curl_easy_cleanup(curl);
        imageFile.close();
    }

    // Use stb_image to load the downloaded image
    int width, height, channels;
    unsigned char* image = stbi_load("downloaded_image.tiff", &width, &height, &channels, 0);

    if (image) {
        // Create an OpenCV Mat from the loaded image data
        cv::Mat imgMat(height, width, channels == 4 ? CV_8UC4 : CV_8UC3, image);

        // Display the image using OpenCV
        cv::imshow("Downloaded Image", imgMat);
        cv::waitKey(0);        

        // Free the image data
        stbi_image_free(image);
        
    } else {
        std::cerr << "Error loading image" << std::endl;
    }

    return 0;
}
