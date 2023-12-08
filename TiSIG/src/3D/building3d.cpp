#include "building3d.h"

#include <cmath>


/////////////////////////////  Building 3D Factory  //////////////////////////////////////

Building3D * Building3DFactory::New() const
{
	// use constructor depending on if we have normals or not
	if (this->normals.size() == 0)
		return new Building3D(this->vertices, this->uv, this->pathTexture, this->symbo);
	return new Building3D(this->vertices, this->normals, this->uv, this->pathTexture, this->symbo);
}
