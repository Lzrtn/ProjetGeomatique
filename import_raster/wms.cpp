// wms.cpp
#include "wms.h"
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;

WMS::WMS(int crs, double north_limit, double south_limit, double east_limit, double west_limit,
         int width, int height)
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

    std::cout<<str_url;
    
    url = strdup(str_url.c_str());
    }

const char* WMS::getUrl() {
    return url;
}

WMS::~WMS(){
}
