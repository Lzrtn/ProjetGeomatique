#include "gtest/gtest.h"
#include "../src/3D/camera.h"

///////////////////////////////     motion 3D     ///////////////////////////////

TEST(camera_3D_motion, test_fail) {
	// arrange
	Camera c;

	// compute
	c.ComputeMPV();

	QVector4D v = {0,0,0,0};
	QVector4D v2 = c.getMVP() * v;


	// assert
	EXPECT_EQ (v2, v);
}

///////////////////////////////    picking 3D     ///////////////////////////////

TEST(camera_3D_picking, test_revert_proj_center) {
	// arange

	// compute

	// assert
}
