//
// Created by yesiot on 19-5-18.
//

#ifndef PAHO_TEST_IMAGE_PROVIDER_H
#define PAHO_TEST_IMAGE_PROVIDER_H

#include "camera_controller.h"

class ImageProvider {

public:
    ImageProvider();
    ImageProvider(const ImageProvider&) = delete;
    ImageProvider& operator=(const ImageProvider&) = delete;

    std::vector<uint8_t > getImage();

private:
    CameraController m_cameraController;
};


#endif //PAHO_TEST_IMAGE_PROVIDER_H
