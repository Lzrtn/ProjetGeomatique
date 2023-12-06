// wmts.cpp
#include "wmts.h"
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <curl/curl.h>
#include <fstream>
#include "crs_converter.hpp"


using namespace std;
using namespace cv;

WMTS::WMTS(int tilematrix, double west_limit, double north_limit, int width, int height, int epsg)
/*
*
*   WMTS Class Constructor
*
*   tilematrix : level of zoom (interval depends on the base link used)
*   west_limit : west coordinate limit of the bounding box
*   north_limit : north coordinate limit of the bounding box
*   width : width of display screen
*   height : height of display screen
*
*/
{
    //String utils 
    string coma              = ",";

    string key_service       = "?SERVICE=";      //SERVICE = type of service you want (WMS/WFS/WMTS/TMS) 
    string key_version       = "&VERSION=";
    string key_request       = "&REQUEST=";      //REQUEST = type of get you want (GetMap/GetTiles/GetCapabilities/GetFeatures) (Warning : the type of get depends of the service you ask for)
    string key_styles        = "&STYLE=";
    string key_layers        = "&LAYER=";        //LAYERS = the layer you want to show
    string key_exceptions    = "&EXCEPTIONS=";
    string key_format        = "&FORMAT=image/"; //FORMAT = format of the image get on the flow (jpeg/png/tiff/geotiff) (Warning : depending on the flow not all the format could be handled)
    string key_tilematrixset = "&TILEMATRIXSET=";
    string key_tilematrix    = "&TILEMATRIX=";
    string key_tilecol       = "&TILECOL=";
    string key_tilerow       = "&TILEROW=";

    //Base url of the flow ("wms-r" for WMS-Raster)
    string link           = "https://wxs.ign.fr/essentiels/geoportail/wmts";    
    string layer          = "ORTHOIMAGERY.ORTHOPHOTOS";
    string exception      = "text/xml"; 
    string format         = "jpeg";   
    string service        = "WMTS";
    string version        = "1.0.0";    
    string request        = "GetTile";
    string style          = "normal";
    string tilematrixset  = "PM";     

    //Utils (coordinate in different projections)
    int tileSize = 256; //Size of one tile
    double initialResolution = 156543.03392804097; 
    double x0 = -20037508.342789;
    double y0 = 20037508.342789; 

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

    std::vector<double> lamb93_5cm_resol = {209715.2, 104857.6, 52428.8, 26214.4, 13107.2, 6553.6, 3276.8, 1638.4, 819.2, 409.6, 204.8, 102.4, 51.2, 25.6, 12.8, 
    6.4, 3.2, 1.6, 0.8, 0.4, 0.2, 0.1, 0.05};    

    //Calculate TLC
    vector<double> coordinates = crs_converter(epsg,north_limit,west_limit,3857);
    double x_tlc = coordinates[1] - x0;
    double y_tlc = y0 - coordinates[2];
    double distance_per_tile = tileSize * initialResolution/pow(2,tilematrix);

    //Calculate number of tiles
    int tilecol_lenght = round(width/tileSize) + 1;
    int tilerow_lenght = round(height/tileSize) + 1;

    //Getting useful tiles
    int tilerow = round(y_tlc/distance_per_tile);
    for (int i = 0; i<tilerow_lenght; i++)
    {
        vector<const char*> vecurl;
        tilerow ++;
        int tilecol = round(x_tlc/distance_per_tile);
        for (int j = 0; j<tilecol_lenght; j++)
        {
            tilecol ++;
            string str_tilerow    = to_string(tilerow);
            string str_tilecol    = to_string(tilecol);

            string str_url = link + key_service + service + key_version + version + key_request + request +
                key_styles + style + key_layers + layer + key_exceptions + exception + key_format + format +         
                key_tilematrixset + tilematrixset + key_tilematrix + str_tilematrix + 
                key_tilecol + str_tilecol + key_tilerow + str_tilerow;
            
            url = strdup(str_url.c_str());
            vecurl.push_back(url);
        }
        tablurl.push_back(vecurl);
    }
}

void WMTS::setURL(const char* newurl){
    this.url = newurl;
}

vector<vector<const char*>> WMTS::getUrl() {
    /*for (size_t i = 0; i <tablurl.size(); ++i) {
            for (size_t j = 0; j < tablurl[i].size(); ++j) {
                cout << i << " " << j << " : " << tablurl[i][j] << endl;
            }
        }*/
    return tablurl;    
}

WMTS::~WMTS(){
}

// Callback function to write the received data to a file
size_t WriteCallbacks(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::ostream* out = static_cast<std::ostream*>(userp);
    out->write(static_cast<char*>(contents), realsize);
    return realsize;
}

void WMTS::getImage(){
    // Initialize libcurl
    CURL* curl = curl_easy_init();

    if (curl) {

        for (size_t i = 0; i <tablurl.size(); ++i) {
            for (size_t j = 0; j < tablurl[i].size(); ++j) {
                // Set the URL to download
                curl_easy_setopt(curl, CURLOPT_URL, tablurl[i][j]);

                // Set up the callback to write the data to a file
                std::ofstream imageFile("../src/data/dowloaded_images/wmts_images/downloaded_wmts_image"+to_string(i)+to_string(j)+".jpeg", std::ios::binary);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbacks);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageFile);

                // Perform the HTTP request
                CURLcode res = curl_easy_perform(curl);

                // Check for errors
                if (res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

                imageFile.close();               
            }
        }
        // Cleanup
        curl_easy_cleanup(curl);        
    } 
}

void WMTS::combine()
{
    int rows = tablurl.size();
    int cols = tablurl[0].size();
    //Verify size
    if (rows <= 0 || cols <= 0) {
        cerr << "Erreur : Nombre de lignes ou de colonnes invalide." << endl;
        return;
    }

    // Create grid
    Mat gridImage(rows * 258, cols * 258, CV_8UC3, Scalar(255, 255, 255)); 

    // Upload and concatenate images
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Build path
            string imagePath = "../src/data/dowloaded_images/wmts_images/downloaded_wmts_image"+to_string(i)+to_string(j)+".jpeg";

            // Upload image
            Mat image = imread(imagePath);
            if (image.empty()) {
                cerr << "Erreur : Impossible de charger l'image " << imagePath << endl;
                return;
            }

            // Copy in grid
            Rect roi(j * 258+1, i * 258+1, 256, 256);
            image.copyTo(gridImage(roi));
        }
    }

    // Save output image
    imwrite("../src/data/dowloaded_images/wmts_combine/combine.jpeg", gridImage);
    
    cout << "Grille d'images créée avec succès"<< endl;
}