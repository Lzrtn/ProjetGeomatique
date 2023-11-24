#include "building3d.h"

#include <cmath>


/////////////////////////////  Building 3D Factory  //////////////////////////////////////

Building3DFactory::Building3DFactory(const std::vector<QVector3D> &position,
									 const std::vector<QVector3D> &normal,
									 const std::vector<QVector2D> &textCoord,
									 const std::string &textPath) :
	position(position), normal(normal), textCoord(textCoord), textPath(textPath)
{}

Building3DFactory::Building3DFactory(const int version)
{
	/*
	 * version 0 and version 1 are same with different origin
	 * regular tetrahedre
	 */
	switch (version)
	{
	case 0:
		position = {
			{ 2, 1,-1}, { 4, 1, 1}, { 4,-1,-1},
			{ 2, 1,-1}, { 4, 1, 1}, { 2,-1, 1},
			{ 4,-1,-1}, { 4, 1, 1}, { 2,-1, 1},
			{ 4,-1,-1}, { 2, 1,-1}, { 2,-1, 1}
		};
		normal = position; // wrong values, but without light, it has no effects
		textCoord = {
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1}
		};
		textPath = ":/cube.png";
		break;

	case 1:
		position = {
			{ -4, 1,-1}, { -2, 1, 1}, { -2,-1,-1},
			{ -4, 1,-1}, { -2, 1, 1}, { -4,-1, 1},
			{ -2,-1,-1}, { -2, 1, 1}, { -4,-1, 1},
			{ -2,-1,-1}, { -4, 1,-1}, { -4,-1, 1}
		};
		normal = position; // wrong values, but without light, it has no effects
		textCoord = {
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1}
		};
		textPath = ":/cube.png";
		break;
	case 2:
		position = {
			{-2, 18, 0}, {-2, 22, 0}, {2, 22, 0},
			{2, 22, 0}, {2, 18, 0}, {-2, 18, 0}
		};
		normal = position;
		textCoord = {
			{0, 0}, {1, 0}, {1, 1},
			{0, 0}, {0, 1}, {1, 1}
		};
		textPath = ":/cube.png";
		break;
	case 3:
	default:
		position = {
			{-2, -2, 5}, {-2,  2, 5}, { 2,  2, 5},
			{-2, -2, 5}, { 2, -2, 5}, { 2,  2, 5},
		};
		normal = position;
		textCoord = {
			{0, 0}, {0, 1}, {1, 1},
			{0, 0}, {1, 0}, {1, 1},
		};
		textPath = ":/rose des vents.png";
		break;
	}

}

Building3D * Building3DFactory::NewBuilding() const
{
	return new Building3D(this->position, this->normal, this->textCoord, this->textPath);
}
