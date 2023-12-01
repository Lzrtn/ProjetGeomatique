#include <gtest/gtest.h>
#include <gdal/gdal_priv.h>
#include <gdal/ogrsf_frmts.h>
#include "../src/2D/geojson.h"

/**
 * @brief Test case for converting GeoJSON to SHP.
 *
 * This test case verifies the functionality of converting a GeoJSON file to SHP format.
 * It opens the GeoJSON file, converts it to SHP, and then opens the resulting SHP file.
 * It compares the number of layers and features in the original GeoJSON and the converted SHP.
 */
TEST(GeoJsonTest, ConvertToShpTest)
{
    
    std::string geojsonFilePath = "../src/data/DONNEES_BDTOPO/Bati/Bati_Lyon5eme_test.geojson";
    GeoJson geojson(geojsonFilePath);
    GDALDataset *poDS = (GDALDataset *)GDALOpenEx(geojsonFilePath.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
    ASSERT_TRUE(poDS != NULL);

    geojson.ConvertToShp();

    GDALDataset *poDSOut = (GDALDataset *)GDALOpenEx(geojson.GetShpFilePath().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
    ASSERT_TRUE(poDSOut != NULL);

    int numLayersIn = poDS->GetLayerCount();
    int numLayersOut = poDSOut->GetLayerCount();

    std::cout << "Number of layers in GeoJSON: " << numLayersIn << std::endl;
    std::cout << "Number of layers in SHP: " << numLayersOut << std::endl;
    EXPECT_EQ(numLayersIn, numLayersOut);

    int numFeaturesIn = poDS->GetLayer(0)->GetFeatureCount();
    int numFeaturesOut = poDSOut->GetLayer(0)->GetFeatureCount();
    std::cout << "Number of features in GeoJSON: " << numFeaturesIn << std::endl;
    std::cout << "Number of features in SHP: " << numFeaturesOut << std::endl;
    EXPECT_EQ(numFeaturesIn, numFeaturesOut);

    GDALClose(poDS);
    GDALClose(poDSOut);
}
