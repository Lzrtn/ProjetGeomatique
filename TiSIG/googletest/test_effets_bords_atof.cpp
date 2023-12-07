#include "gtest/gtest.h"

/**
 * these test are made to find why atof fail with reading "." in app (src.pro) and not in test (googletest.pro)
 *
 * Conclusion: an error with Qt default parameters in Linux. Others tests are disabled and not maintened
 */
/*
#include "../src/outils/dbmanager.h"
#include "../src/outils/docker.h"
#include "../src/outils/executor.h"
#include "../src/2D/geotiff.h"
#include "../src/outils/docker.h"

#include <gdal/gdal_priv.h>
#include <gdal/ogrsf_frmts.h>
#include "../src/2D/geojson.h"
*/

#include <QApplication>


TEST(effets_bords_atof, atof_only) {
	EXPECT_EQ(atof("3.6"),3.6);
}

/*
TEST(effets_bords_atof, test_with_docker) {
	EXPECT_EQ(atof("3.6"),3.6);
	std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
	EXPECT_EQ(atof("3.6"),3.6);
	Docker docker(pathDockerFile);
	EXPECT_EQ(atof("3.6"),3.6);
	DbManager manager("database2D",docker.getIpAdress());
	EXPECT_EQ(atof("3.6"),3.6);
}


TEST(effets_bords_atof, test_with_executor){
	EXPECT_EQ(atof("3.6"),3.6);
	Executor executor;
	EXPECT_EQ(atof("3.6"),3.6);
	executor.exec("echo 'hello world!'");
	EXPECT_EQ(atof("3.6"),3.6);
}

TEST(effets_bords_atof, test_with_geotiff)
{
	EXPECT_EQ(atof("3.6"),3.6);
	Geotiff geotiff("../src/data/DONNEES_BDORTHO/Lyon5eme_20m.tif");
	EXPECT_EQ(atof("3.6"),3.6);
	ASSERT_EQ(geotiff.CalculateNumberBands(), 4);
	EXPECT_EQ(atof("3.6"),3.6);
}


TEST(effets_bords_atof, test_with_GeoJson)
{
	EXPECT_EQ(atof("3.6"),3.6);

	std::string geojsonFilePath = "../src/data/DONNEES_BDTOPO/Bati/Bati_Lyon5eme_test.geojson";
	EXPECT_EQ(atof("3.6"),3.6);
	GeoJson geojson(geojsonFilePath);
	EXPECT_EQ(atof("3.6"),3.6);
	GDALDataset *poDS = (GDALDataset *)GDALOpenEx(geojsonFilePath.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
	EXPECT_EQ(atof("3.6"),3.6);
	ASSERT_TRUE(poDS != NULL);
	EXPECT_EQ(atof("3.6"),3.6);

	EXPECT_EQ(atof("3.6"),3.6);
	geojson.ConvertToShp();
	EXPECT_EQ(atof("3.6"),3.6);

	EXPECT_EQ(atof("3.6"),3.6);
	GDALDataset *poDSOut = (GDALDataset *)GDALOpenEx(geojson.GetShpFilePath().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
	EXPECT_EQ(atof("3.6"),3.6);
	ASSERT_TRUE(poDSOut != NULL);
	EXPECT_EQ(atof("3.6"),3.6);

	EXPECT_EQ(atof("3.6"),3.6);
	int numLayersIn = poDS->GetLayerCount();
	EXPECT_EQ(atof("3.6"),3.6);
	int numLayersOut = poDSOut->GetLayerCount();
	EXPECT_EQ(atof("3.6"),3.6);

	EXPECT_EQ(atof("3.6"),3.6);
	std::cout << "Number of layers in GeoJSON: " << numLayersIn << std::endl;
	EXPECT_EQ(atof("3.6"),3.6);
	std::cout << "Number of layers in SHP: " << numLayersOut << std::endl;
	EXPECT_EQ(atof("3.6"),3.6);
	EXPECT_EQ(numLayersIn, numLayersOut);
	EXPECT_EQ(atof("3.6"),3.6);

	EXPECT_EQ(atof("3.6"),3.6);
	int numFeaturesIn = poDS->GetLayer(0)->GetFeatureCount();
	EXPECT_EQ(atof("3.6"),3.6);
	int numFeaturesOut = poDSOut->GetLayer(0)->GetFeatureCount();
	EXPECT_EQ(atof("3.6"),3.6);
	std::cout << "Number of features in GeoJSON: " << numFeaturesIn << std::endl;
	EXPECT_EQ(atof("3.6"),3.6);
	std::cout << "Number of features in SHP: " << numFeaturesOut << std::endl;
	EXPECT_EQ(atof("3.6"),3.6);
	EXPECT_EQ(numFeaturesIn, numFeaturesOut);
	EXPECT_EQ(atof("3.6"),3.6);

	EXPECT_EQ(atof("3.6"),3.6);
	GDALClose(poDS);
	EXPECT_EQ(atof("3.6"),3.6);
	GDALClose(poDSOut);
	EXPECT_EQ(atof("3.6"),3.6);
}
*/

TEST(effets_bords_atof, test_with_qapplication)
{
	int argc = 1;
	char *argv[] = {"a"};
	EXPECT_EQ(atof("3.6"),3.6);
	QApplication a(argc, argv);
	//EXPECT_EQ(atof("3.6"),3.6);	// in linux, qt have default parameters witch are modifying atof function
	setlocale(LC_NUMERIC,"C");		// change default parameters to resolve atof function
	EXPECT_EQ(atof("3.6"),3.6);
}
