#include "gtest/gtest.h"
#include "../src/3D/object3d.h"
#include "../src/3D/object3d.cpp" // to use readObj function

//#include <iostream>

///////////////////////////////   read .obj file 3D   ///////////////////////////////

TEST(readObjFiles, testReadMinimalObj) {
	// arrange
	std::string path = "../src/data/Tests/cube.obj";
	std::vector<QVector3D> vertices, normals;
	std::vector<QVector2D> uv;

	// compute
	readObj(path, vertices, normals, uv);

	// assert
	EXPECT_TRUE(vertices.size() != 0);
	EXPECT_TRUE(normals.size() != 0);
	EXPECT_TRUE(uv.size() != 0);
//	EXPECT_EQ (v2, v);
}


TEST(readObjFiles, testReadUnvalidPath) {
	// arrange
	std::string path = "valid/path/syntax/but/refered/in/any/folder/nor/file/and/use/extention.obj";
	std::vector<QVector3D> vertices, normals;
	std::vector<QVector2D> uv;

	bool error = false;
	// compute throw
	try {
		readObj(path, vertices, normals, uv);
	} catch (std::runtime_error /*e*/) {
		error = true;
	}
	EXPECT_TRUE(error);
}


TEST(readObjFiles, testReadObjVertice) {
	// arrange
	std::string path = "../src/data/Tests/cube.obj";
	std::vector<QVector3D> vertices, normals;
	std::vector<QVector2D> uv;

	// compute
	readObj(path, vertices, normals, uv);

	// assert
	EXPECT_TRUE(vertices.at(0).x() == 1);
	EXPECT_TRUE(vertices.at(0).y() == 1);
	EXPECT_TRUE(vertices.at(0).z() == -1);

	EXPECT_TRUE(vertices.at(1).x() == 1);
	EXPECT_TRUE(vertices.at(1).y() == -1);
	EXPECT_TRUE(vertices.at(1).z() == -1);
}

TEST(readObjFiles, testReadObjUv) {
	// arrange
	std::string path = "../src/data/Tests/cube.obj";
	std::vector<QVector3D> vertices, normals;
	std::vector<QVector2D> uv;

	// compute
	readObj(path, vertices, normals, uv);

	// assert
	EXPECT_TRUE(abs(uv.at(1).x() - 0.749279) < 1e-5);
	EXPECT_TRUE(abs(uv.at(1).y() - 0.501284) < 1e-5);
}


TEST(readObjFiles, testReadObjNormals) {
	// arrange
	std::string path = "../src/data/Tests/cube.obj";
	std::vector<QVector3D> vertices, normals;
	std::vector<QVector2D> uv;

	// compute
	readObj(path, vertices, normals, uv);

	// assert
	EXPECT_TRUE(abs(normals.at(12).x() + 0.000001) < 1e-5);
	EXPECT_TRUE(abs(normals.at(12).y() - 0.000000) < 1e-5);
	EXPECT_TRUE(abs(normals.at(12).z() - 1.000000) < 1e-5);
}
