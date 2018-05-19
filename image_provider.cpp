//
// Created by yesiot on 19-5-18.
//

#include "image_provider.h"

ImageProvider::ImageProvider() {
}

std::vector<uint8_t > ImageProvider::getImage()   {
    return m_cameraController.MakeSinglePicture();
}