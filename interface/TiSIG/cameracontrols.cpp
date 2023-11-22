#include "cameracontrols.h"
#include <iostream>

CameraControls::CameraControls()
{

}

void CameraControls::update(const float dt)
{
	if (this->camera == nullptr || this->widget == nullptr) return;
	std::cout << dt << std::endl;
}
