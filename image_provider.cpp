//
// Created by yesiot on 19-5-18.
//

#include "image_provider.h"

ImageProvider::ImageProvider() :
        m_cameraController(640, 480)
{
}

std::vector<uint8_t > ImageProvider::getImage()   {
    return m_cameraController.MakeSinglePicture();
}