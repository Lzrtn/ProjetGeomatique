#include "mnt3d.h"


MNT3D *MNT3DFactory::NewBuilding() const
{
	return new MNT3D(this->pathObj, this->pathTexture);
}
