#include "gtest/gtest.h"
#include "../TiSIG/camera.cpp"

TEST(camera_3D_motion, test_fail) {
	//arrange
	Camera c;

	c.ComputeMPV();

	QVector4D v = {0,0,0,0};
	QVector4D v2 = c.getMVP() * v;


	//assert
	EXPECT_EQ (v2, v);
}

