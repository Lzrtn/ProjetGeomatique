#include <gtest/gtest.h>
#include "../src/2D/geotiff.h"
#include "../src/outils/docker.h"

/**
 * The function "TestCalculateNumberBands" tests the "CalculateNumberBands" method of the "Geotiff"
 * class and asserts that the result is equal to 1.
 *
 * @param  - Test case name: GeotiffTest
 */
TEST(GeotiffTest, TestCalculateNumberBands)
{
    Geotiff geotiff("../src/data/DONNEES_BDORTHO/Lyon5eme_20m.tif");
    ASSERT_EQ(geotiff.CalculateNumberBands(), 3);
}

/**
 * The function "TestCalculateImageDimensions" tests the "CalculateImageDimensions" method of the
 * Geotiff class by asserting that the returned image dimensions match the expected values.
 *
 * @param  - Test case name: GeotiffTest
 */
TEST(GeotiffTest, TestCalculateImageDimensions)
{
    Geotiff geotiff("../src/data/DONNEES_BDORTHO/Lyon5eme_20m.tif");
    ASSERT_EQ(geotiff.CalculateImageDimensions(), std::vector<int>({2598, 1744}));
}

/**
 * The function tests if the CalculateExtent() method of the Geotiff class returns a vector of 4 double
 * coordinates.
 *
 * @param  - Test case name: GeotiffTest
 */
TEST(GeotiffTest, TestCalculateExtent)
{
    Geotiff geotiff("../src/data/DONNEES_BDORTHO/Lyon5eme_20m.tif");
    // assert if the extent return a vector of 4 doubles coordinates
    ASSERT_EQ(geotiff.CalculateExtent().size(), 4);
};

/**
 * The function "TestCalculateResolution" tests the "CalculateResolution" method of the Geotiff class
 * by comparing the returned resolution values with the expected values.
 * @param  The bellow code snippet is a unit test case for testing the `CalculateResolution()` method of
 * the `Geotiff` class.

 */
TEST(GeotiffTest, TestCalculateResolution)
{
    Geotiff geotiff("../src/data/DONNEES_BDORTHO/Lyon5eme_20m.tif");
    ASSERT_EQ(geotiff.CalculateResolution(), std::vector<double>({2.0002, -1.9998}));
}


TEST(GeotiffTest, TestWriteGeotiffAndMetadataToPostgis)
{
//     Creating container
    std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
    Docker docker(pathDockerFile);
//     Get the Ip Adress
    std::string ipAdress = docker.getIpAdress();
    std::cout << ipAdress << std::endl;

//     PostGreSQL Connection to the first database
    DbManager test("database2D", ipAdress);
    pqxx::connection conn(test.getString());
    Geotiff geotiff("../src/data/DONNEES_BDORTHO/Lyon5eme_20m.tif");
    geotiff.CalculateExtent();
    geotiff.CalculateResolution();
    geotiff.WriteGeotiffAndMetadataToPostgis(test);

    int rows_before = geotiff.GetNumberOfImagesStored(test);
    geotiff.WriteGeotiffAndMetadataToPostgis(test);
    int rows_after = geotiff.GetNumberOfImagesStored(test);
    ASSERT_EQ(rows_after, rows_before + 1);
}


/**
 * The function "TestCalculateOrigin" tests the "CalculateOrigin" method of the "Geotiff" class by
 * asserting that the size of the returned origin vector is equal to 2.
 *
 * @param  The bellow code is a unit test for a Geotiff class. It tests the CalculateOrigin() method of
 * the Geotiff class by checking if the size of the returned origin vector is equal to 2. The Geotiff
 * object is initialized with the file path "/home/formation/Documents/d
 */
TEST(GeotiffTest, TestCalculateOrigin)
{
    Geotiff geotiff("../src/data/DONNEES_BDORTHO/Lyon5eme_20m.tif");
    ASSERT_EQ(geotiff.CalculateOrigin().size(), 2);
}
