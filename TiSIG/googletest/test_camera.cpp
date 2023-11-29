#include "gtest/gtest.h"
#include "../src/3D/camera.h"

//#include <iostream>

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
	Camera camera;

	QVector3D pReel = {0, 0, 0};

	camera.ResizeView(500, 500);
	camera.setAngleH(0);
	camera.setAngleV(0);
	camera.setPosition({0,0,10});

	// compute

	QVector3D pScreen = camera.ProjToScreen(pReel);
	QVector3D pReel2 = camera.ProjFromScreen(pScreen);

	//*
	std::cout << pReel.x() << " " << pReel.y() << " " << pReel.z() << "\n";
	std::cout << pScreen.x() << " " << pScreen.y() << " " << pScreen.z() << "\n";
	std::cout << pReel2.x() << " " << pReel2.y() << " " << pReel2.z() << "\n";
	//*/

	// assert
	EXPECT_EQ (pReel2, pReel);
}

TEST(camera_3D_picking, test_revert_proj_excenter) {
	// arange
	Camera camera;

	QVector3D pReel = {1.0, 2.0, 1.0};

	camera.ResizeView(500, 500);
	camera.setAngleH(0);
	camera.setAngleV(0);
	camera.setPosition({0,0,10});

	// compute

	camera.ComputeMPV();

	QVector3D pScreen = camera.ProjToScreen(pReel);
	QVector3D pReel2 = camera.ProjFromScreen(pScreen);

	/*
	std::cout << pReel.x() << " " << pReel.y() << " " << pReel.z() << "\n";
	std::cout << pScreen.x() << " " << pScreen.y() << " " << pScreen.z() << "\n";
	std::cout << pReel2.x() << " " << pReel2.y() << " " << pReel2.z() << "\n";
	//*/

	// assert
	EXPECT_TRUE ((pReel2 - pReel).length() < 1e-3);	// due to rounds errors, vectors are not strictly equals
}

TEST(camera_3D_picking, test_proj_excenter) {
	// arange
	Camera camera;

	QVector3D pScr = {1.0, 2.0, 1.0};

	camera.ResizeView(500, 500);
	camera.setAngleH(0);
	camera.setAngleV(0);
	camera.setPosition({0,0,10});

	// compute

	camera.ComputeMPV();

	QVector3D pRe = camera.ProjFromScreen(pScr);
	QVector3D pScr2 = camera.ProjToScreen(pRe);

	// assert
	EXPECT_TRUE ((pScr - pScr2).length() < 1e-3);	// due to rounds errors, vectors are not strictly equals
}

TEST(camera_3D_picking, test_proj_screen_center) {
	// arange
	Camera camera;

	QVector3D pScreen = {250, 250, 1};

	camera.ResizeView(500, 500);
	camera.setAngleH(0);
	camera.setAngleV(0);
	camera.setPosition({0,0,10});

	// compute

	camera.ComputeMPV();

	QVector3D pReel = camera.ProjFromScreen(pScreen);

	/*
	std::cout << pReel.x() << " " << pReel.y() << " " << pReel.z() << "\n";
	std::cout << pScreen.x() << " " << pScreen.y() << " " << pScreen.z() << "\n";
	//*/

	// assert
	EXPECT_EQ(pReel.x(), 0);
	EXPECT_EQ(pReel.y(), 0);
}


TEST(emprise, test_compute_ground_emprise) {
	// arange
	Emprise e;
	e.a = {5, 5, 5};
	e.e = {6, 6, 6};
	e.b = {5, 5, 0};
	e.f = {6, 6, 6};
	e.c = {5, 5, 7};
	e.g = {6, 5, 0};

	// compute
	e.updateGroundCoordinates();

	// assert
	EXPECT_TRUE((e.g_a - QVector3D(0, 0, 0)).length() < 1e-5);
	EXPECT_TRUE((e.g_b - QVector3D(5, 5, 0)).length() < 1e-5);
	EXPECT_TRUE((e.g_c - QVector3D(6, 5, 0)).length() < 1e-5);
}
