#include "exempleobject3dstorage.h"
#include "building3d.h"
#include "mnt3d.h"

ExempleObject3DStorage::ExempleObject3DStorage()
{
	this->buildings = {};
	std::vector<QVector3D> vertices, normals;
	std::vector<QVector2D> uv;


	/*------------------------------  add tetrahedres -----------------------------*/
	vertices = {
		{ 2, 1,-1}, { 4, 1, 1}, { 4,-1,-1},
		{ 2, 1,-1}, { 4, 1, 1}, { 2,-1, 1},
		{ 4,-1,-1}, { 4, 1, 1}, { 2,-1, 1},
		{ 4,-1,-1}, { 2, 1,-1}, { 2,-1, 1}
	};
	normals = {
		{-1, 1,-1}, { 1, 1, 1}, { 1,-1,-1},
		{-1, 1,-1}, { 1, 1, 1}, {-1,-1, 1},
		{ 1,-1,-1}, { 1, 1, 1}, {-1,-1, 1},
		{ 1,-1,-1}, {-1, 1,-1}, {-1,-1, 1}
	};
	uv = {
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1}
	};
	this->buildings[0] = new Building3DFactory(vertices, normals, uv, ":/cube.png");

	vertices = {
		{ -4, 1,-1}, { -2, 1, 1}, { -2,-1,-1},
		{ -4, 1,-1}, { -2, 1, 1}, { -4,-1, 1},
		{ -2,-1,-1}, { -2, 1, 1}, { -4,-1, 1},
		{ -2,-1,-1}, { -4, 1,-1}, { -4,-1, 1}
	};
	// normals same
	// uv same
	this->buildings[1] = new Building3DFactory(vertices, normals, uv, ":/cube.png");

	/*------------------------------ add square (45°) -----------------------------*/
	vertices = {
		{-2, 18, 5}, {-2, 22, 5}, { 2, 22, 5},
		{-2, 18, 5}, { 2, 18, 5}, { 2, 22, 5},
	};
	normals = {
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
	};
	uv = {
		{0, 0}, {0, 1}, {1, 1},
		{0, 0}, {1, 0}, {1, 1},
	};
	this->buildings[2] = new Building3DFactory(vertices, normals, uv, ":/cube.png");

	/*------------------------------ square (compass) -----------------------------*/
	vertices = {
		{-2, -2, 5}, {-2,  2, 5}, { 2,  2, 5},
		{-2, -2, 5}, { 2, -2, 5}, { 2,  2, 5},
	};
	normals = {
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
	};
	uv = {
		{0, 0}, {0, 1}, {1, 1},
		{0, 0}, {1, 0}, {1, 1},
	};
	this->buildings[3] = new Building3DFactory(vertices, normals, uv, ":/rose des vents.png");

	/*------------------------------    MNT (cube)   ------------------------------*/
	this->buildings[4] = new MNT3DFactory("../src/data/Tests/cube.obj", ":/cube.png");

	/*------------------------------ Set translation ------------------------------*/
	this->setTranslation({5, 0, 0});
}

ExempleObject3DStorage::~ExempleObject3DStorage()
{
	/*------------------------------ free memory     ------------------------------*/
	for (auto pair: this->buildings) {
		delete pair.second;
	}
}

void ExempleObject3DStorage::GetObjectsInEmprise(
		const Emprise &/*emprise*/,
		std::map<int, Object3DFactory *> &new_buildings,
		std::vector<int> &show_objects,
		std::vector<int> &forget_objects)
{
	new_buildings.clear();

	// compute displayed buildings with emprise
	show_objects = {0, 1, 2, 3, 4};

	// give new buildings
	for (int id: show_objects) {
		new_buildings[id] = buildings[id];
	}

	// building id to forget
	forget_objects={};
}
