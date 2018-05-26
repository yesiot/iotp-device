#include <iostream>

#include "mqtt_engine.h"
#include "image_provider.h"
#include "config_reader.h"



void showUsage() {
    std::cout << "Usage:\n"\
                 "paho_test <config file name>\n\n"\
                 "config file in the json format containing following keys:\n"\
                 " name     - client name"
                 " host     - address of the mqtt broker\n"\
                 " port     - port number used by mqtt service\n"\
                 " user     - username used in the mqtt broker authentication\n"\
                 " password - password used for broker authentication" << std::endl;
}

std::atomic<bool> sendPicture{false};

void handleIncomingMessage(const std::string& topic, const void* payload, size_t payloadSize) {
    std::cout << "New message arrived: " << topic << std::endl;
    std::string msg = std::string((const char*)payload, payloadSize);

    if(msg == "on") {
        sendPicture = true;
    } else {
        sendPicture = false;
    }
}

int main(int argc, char* argv[])
{
    std::cout << "PAHO TEST V 1.0" << std::endl;

    if(argc < 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        showUsage();
        return -1;
    }

    ConfigReader configReader;
    configReader.readConfigFile(argv[1]);

    auto hostAddress = configReader.getHostAddress();
    auto portNr = configReader.getPortNr();
    auto deviceName = configReader.getClientName();
    auto userName = configReader.getUserName();
    auto userPassword = configReader.getPassword();

    MqttEngine    mqttEngine(deviceName, hostAddress, portNr);
    mqttEngine.onNewMessage = handleIncomingMessage;

    ImageProvider imageProvider;

    if(!userName.empty()) {
        mqttEngine.SetPassword(userName, userPassword);
    }

    if(!mqttEngine.Connect()) {
        std::cerr << "Cannot connect to the MQTT server" << std::endl;
        return -1;
    }

    try {

        const std::string aliveStr = "ALIVE";
        auto cnt = 0;
        std::vector<unsigned char> image;


        while(true) {
            std::cout << "." << std::endl;

            mqttEngine.Publish("status", aliveStr.c_str(), aliveStr.size());
            std::string cntStr = std::to_string(cnt++);
            mqttEngine.Publish("counter", cntStr.c_str(), cntStr.size());


            if(sendPicture) {
                image = imageProvider.getImage();
                mqttEngine.Publish("picture", image.data(), image.size());
            }
            else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }


        }

        mqttEngine.Disconnect();


    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << " ["
                  << exc.get_reason_code() << "]" << std::endl;
        return 1;
    }

    return 0;
}
