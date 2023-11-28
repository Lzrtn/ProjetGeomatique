// wmts.cpp
#include "wmts.h"
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;

WMTS::WMTS(string tilematrixset, int tilematrix, 
    double north_limit, double west_limit)
{
    //String utils 
    string coma              = ",";

    string key_layers        = "?LAYER=";       //LAYERS = the layer you want to show
    string key_exceptions    = "&EXCEPTIONS=";
    string key_format        = "&FORMAT=image/"; //FORMAT = format of the image get on the flow (jpeg/png/tiff/geotiff) (Warning : depending on the flow not all the format could be handled)
    string key_service       = "&SERVICE=";      //SERVICE = type of service you want (WMS/WFS/WMTS/TMS) 
    string key_version       = "&VERSION=";
    string key_request       = "&REQUEST=";      //REQUEST = type of get you want (GetMap/GetTiles/GetCapabilities/GetFeatures) (Warning : the type of get depends of the service you ask for)
    string key_styles        = "&STYLES=";
    string key_crs           = "&CRS=EPSG:";     //CRS = EPSG: projection key (integer) 
    string key_tilematrixset = "&TILEMATRIXSET=";
    string key_tilematrix    = "&TILEMATRIX=";
    string key_tilecol       = "&TILECOL=";
    string key_tilerow       = "&TILEROW=";

    //Top Left Corner utils (coordinate in different projections)
    double x0_tlc_lamb93 = 0.0000000000000000;
    double y0_tlc_lamb93 = 12000000.0000000000000000;

    //Convert into string for WMTS request
    string str_north = to_string(north_limit);
    /*string str_south = to_string(south_limit);
    string str_east  = to_string(east_limit);*/
    string str_west  = to_string(west_limit);
    string str_tilematrix = to_string(tilematrix);

    //Vector to convert in tilecol and tilerow
    std::vector<double> lamb93_10cm_resol = {104579.2245498940, 52277.5323537905, 26135.4870785954, 13066.8913818000, 6533.2286041135, 3266.5595244627, 1633.2660045974,
    816.6295549860, 408.3139146768, 204.1567415109, 102.0783167832, 51.0391448966, 25.5195690743, 12.7597836936, 6.3798916360, 3.1899457653, 1.5949728695, 
    0.7974864315, 0.3987432149, 0.1993716073, 0.0996858037, 0.0498429018};   

    //Calculate TLC
    double x_tlc = west_limit - x0_tlc_lamb93;
    double y_tlc = north_limit - y0_tlc_lamb93;
    double distance_per_tile = 256 * lamb93_10cm_resol[tilematrix];
    int tilecol = round(x_tlc/distance_per_tile);
    int tilerow = round(y_tlc/distance_per_tile);

    //Base url of the flow ("wms-r" for WMS-Raster)
    string link           = "https://data.geopf.fr/wmts";    
    string layer          = "ORTHOIMAGERY.ORTHOPHOTOS";
    string exception      = "text/xml"; 
    string format         = "png";   
    string service        = "WMTS";
    string version        = "1.0.0";    
    string request        = "GetTile";
    string style          = "normal";
    tilematrixset         = "PM"; 
    string str_tilecol    = to_string(tilecol);
    string str_tilerow    = to_string(tilerow);
    
    string str_url = link + key_layers + layer + key_exceptions + exception + key_format + format + 
                key_service + service + key_version + version + key_request + request + key_styles + style
                + key_tilematrixset + tilematrixset + key_tilematrix + str_tilematrix + key_tilecol + str_tilecol
                + key_tilerow + str_tilerow;
    
    std::cout<<str_url;
    url = strdup(str_url.c_str());
}


const char* WMTS::getUrl() {
    return url;
}

WMTS::~WMTS(){
}