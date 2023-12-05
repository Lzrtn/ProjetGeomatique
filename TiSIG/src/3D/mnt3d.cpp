#include "mnt3d.h"


MNT3D *MNT3DFactory::New() const
{
	return new MNT3D(this->pathObj, this->pathTexture);
}
