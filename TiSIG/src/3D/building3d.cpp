#include "building3d.h"

#include <cmath>


/////////////////////////////  Building 3D Factory  //////////////////////////////////////


Building3DFactory::Building3DFactory(const int version)
{
	/*
	 * version 0 and version 1 are same with different origin: regular tetrahedre
	 *
	 * 2: building seen when camera vangle is 45°
	 *
	 * 3: square with compas texture to control axis orientation
	 */
	switch (version)
	{
	case 0:
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
		pathTexture = ":/cube.png";
		break;

	case 1:
		vertices = {
			{ -4, 1,-1}, { -2, 1, 1}, { -2,-1,-1},
			{ -4, 1,-1}, { -2, 1, 1}, { -4,-1, 1},
			{ -2,-1,-1}, { -2, 1, 1}, { -4,-1, 1},
			{ -2,-1,-1}, { -4, 1,-1}, { -4,-1, 1}
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
		pathTexture = ":/cube.png";
		break;
	case 2:
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
		pathTexture = ":/cube.png";
		break;
	case 3:
	default:
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
		pathTexture = ":/rose des vents.png";
		break;
	}
}

Building3D * Building3DFactory::New() const
{
	// use constructor depending on if we have normals or not
	if (this->normals.size() == 0)
		return new Building3D(this->vertices, this->uv, this->pathTexture);
	return new Building3D(this->vertices, this->normals, this->uv, this->pathTexture);
}
