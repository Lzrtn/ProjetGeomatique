// wms.cpp
#include "wms.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#include <fstream>


using namespace std;

WMS::WMS(int crs, double north_limit, double south_limit, double east_limit, double west_limit,
         int width, int height)
/*
*
*   WMTS Class Constructor
*
*   crs : coordinate reference systeme of north,south,est and west coordinate limit
*   north_limit : north coordinate limit of the bounding box
*   south_limit : south coordinate limit of the bounding box
*   east_limit : east coordinate limit of the bounding box
*   west_limit : west coordinate limit of the bounding box
*   width : width of display screen
*   height : height of display screen
*
*/
{
    //String utils 
    string coma           = ",";

    string key_layers     = "?LAYERS=";       //LAYERS = the layer you want to show
    string key_exceptions = "&EXCEPTIONS=";
    string key_format     = "&FORMAT=image/"; //FORMAT = format of the image get on the flow (jpeg/png/tiff/geotiff) (Warning : depending on the flow not all the format could be handled)
    string key_service    = "&SERVICE=";      //SERVICE = type of service you want (WMS/WFS/WMTS/TMS) 
    string key_version    = "&VERSION=";
    string key_request    = "&REQUEST=";      //REQUEST = type of get you want (GetMap/GetTiles/GetCapabilities/GetFeatures) (Warning : the type of get depends of the service you ask for)
    string key_styles     = "&STYLES=";
    string key_crs        = "&CRS=EPSG:";     //CRS = EPSG: projection key (integer) 
    string key_bbox       = "&BBOX=";         //BBOX = south limit, west limit, north limit, east limit (double)
    string key_width      = "&WIDTH=";        //WIDTH = image width in pixels (integer)
    string key_height     = "&HEIGHT=";       //HEIGHT = image width in pixels (integer)
   

    //Convert into string for WMS request
    string str_north = to_string(north_limit);
    string str_south = to_string(south_limit);
    string str_east  = to_string(east_limit);
    string str_west  = to_string(west_limit);
    string str_width = to_string(width);
    string str_heigh = to_string(height);
    string str_crs   = to_string(crs);

    //Base url of the flow ("wms-r" for WMS-Raster)
    string link           = "https://data.geopf.fr/wms-r";    
    string layer          = "OI.OrthoimageCoverage";
    string exception      = "text/xml"; 
    string format         = "png";   
    string service        = "WMS";
    string version        = "1.3.0";    
    string request        = "GetMap";
    string bbox           = key_bbox+str_south+coma+str_west+coma+str_north+coma+str_east;
    string width_request  = key_width+str_width;
    string height_request = key_height+str_heigh;

    // Concatenate using the + operator
    string str_url = link + key_layers + layer + key_exceptions + exception + key_format + format + 
                key_service + service + key_version + version + key_request + request + key_styles + 
                key_crs + str_crs + bbox + width_request + height_request;

    std::cout<<url<<"\n";
    
    url = strdup(str_url.c_str());
}

const char* WMS::getUrl() {
    return url;
}

WMS::~WMS() {
}

// Callback function to write the received data to a file
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::ostream* out = static_cast<std::ostream*>(userp);
    out->write(static_cast<char*>(contents), realsize);
    return realsize;
}

void WMS::getImage()
    {
    // Initialize libcurl
    CURL* curl = curl_easy_init();

    if (curl) {

        // Set the URL to download
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set up the callback to write the data to a file
        std::ofstream imageFile("dowloaded_images/wms_images/downloaded_wms_image.jpeg", std::ios::binary);
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
    }

string WMS::getPath() {
        return "dowloaded_images/wms_images/downloaded_wms_image.jpeg";
    }
