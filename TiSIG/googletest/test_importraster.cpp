#include "gtest/gtest.h"
#include "../src/2D/wmts.h"
#include "../src/2D/wms.h"
#include <cmath>

TEST(WMSTest, constructor_Default){
    WMS wmsInstance(4326, 45.98624572753906, 45.98514572753706, 4.98760461807251, 4.98650461807051, 256, 256);
    EXPECT_EQ(wmsInstance.getUrl(),"https://data.geopf.fr/wms-r?LAYERS=OI.OrthoimageCoverage&EXCEPTIONS=text/xml&FORMAT=image/jpeg&SERVICE=WMS&VERSION=1.3.0&REQUEST=GetMap&STYLES=&CRS=EPSG:4326&BBOX=45.98624572753906,45.98514572753706,4.98760461807251,4.98650461807051&WIDTH=256&HEIGHT=256");
}

TEST(WMTSTest, size_tablurl){
    WMTS wmtsInstance(14, 536227.222852, 5739265.686868, 1080, 1080);

    int table_size = wmtsInstance.getUrl().size();

    int matrix_size = round(1080/256) +1;

    EXPECT_EQ(table_size,matrix_size);
}