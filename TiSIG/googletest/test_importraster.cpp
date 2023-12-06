#include "gtest/gtest.h"
#include "../src/2D/wmts.h"
#include "../src/2D/wms.h"
#include <cmath>
 
/*--------------------------------------------------WMS-----------------------------------------------------------*/

TEST(WMSTest, size_image){

    //Initialize wms
    WMS wmsInstance(4326,"48.38545104","48.34956960","3.30486151","3.25167353", 256, 256);
    wmsInstance.getImage();
    std::string path = wmsInstance.getPath();
    cv::Mat image = cv::imread(path);    

    //Getting image size
    int width = image.cols;
    int height = image.rows;

    std::tuple<int,int> size(width,height);
    std::tuple<int,int> size_test(256,256);

    /*std::cout << "Image width : " << width << " pixels\n" << std::endl;
    std::cout << "Image height : " << height << " pixels\n" << std::endl;*/

    EXPECT_EQ(size,size_test);
}

TEST(WMSTest, constructor_Default){

    //Initialize wms
    WMS wmsInstance(4326,"47.38545104","47.34956960","3.30486151","3.25167353", 256, 256);
    //Getting url
    const char* url = wmsInstance.getUrl();
    //Initialize other url for equivalence test
    const char* url_test = "https://data.geopf.fr/wms-r?LAYERS=OI.OrthoimageCoverage&EXCEPTIONS=text/xml&FORMAT=image/png&SERVICE=WMS&VERSION=1.3.0&REQUEST=GetMap&STYLES=&CRS=EPSG:4326&BBOX=47.34956960,3.25167353,47.38545104,3.30486151&WIDTH=256&HEIGHT=256";
    //std::cout<<url<<"\n"<<url_test<<"\n-------------------------------\n";

    EXPECT_STREQ(url,url_test);
}

/*--------------------------------------------------WMTS----------------------------------------------------------*/

TEST(WMTSTest, size_tablurl){

    //Initialize WMTS   
    WMTS wmtsInstance(14, 536227.222852, 5739265.686868, 1080, 1080,3857);
    //Getting table_size
    int table_size = wmtsInstance.getUrl().size();
    //Calculating size of table for 1080x1080 pixel image 
    int matrix_size = round(1080/256) +1;
    //std::cout<<table_size<<"\n"<<matrix_size;
    EXPECT_EQ(table_size,matrix_size);
}



TEST(WMTSTest, constructor_Default){

    //Initialize wms
    WMTS wmtsInstance(14, 536227.222852, 5739265.686868, 1080, 1080,3857);
    //Getting url
    const char* url = wmtsInstance.getUrl()[0][2];
    //Initialize other url for equivalence test
    const char* url_test = "https://wxs.ign.fr/essentiels/geoportail/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile&STYLE=normal&LAYER=ORTHOIMAGERY.ORTHOPHOTOS&EXCEPTIONS=text/xml&FORMAT=image/jpeg&TILEMATRIXSET=PM&TILEMATRIX=14&TILECOL=8414&TILEROW=5847";
    //std::cout<<url<<"\n"<<url_test<<"\n-------------------------------\n";

    EXPECT_STREQ(url,url_test);
}

