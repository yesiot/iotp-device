#include <iostream>
#include "mqtt/async_client.h"
#include "mqtt/client.h"

#include "camera_controller.h"

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

int main(int argc, char* argv[])
{
    if(argc < 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        showUsage();
        return -1;
    }

    ConfigReader configReader;
    configReader.readConfigFile(argv[1]);

    auto deviceName = configReader.getClientName();
    auto userName = configReader.getUserName();
    auto userPassword = configReader.getPassword();

    mqtt::client cli(configReader.getConnectionUrl(), deviceName);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    if(!userName.empty()) {
        connOpts.set_user_name(userName);
        connOpts.set_password(userPassword);
    }

    std::string statusStr = "DEAD";

    //Last will - message that is send by broker if connection breaks
    connOpts.set_will({deviceName + "/status", statusStr.c_str(), statusStr.size()});

    CameraController cameraController;


    try {

        cli.connect(connOpts);

        auto cnt = 0;
        std::vector<unsigned char> data;

        while(true) {

            statusStr = "ALIVE";
            cli.publish(deviceName + "/status", statusStr.c_str(), statusStr.size());

            // Now try with itemized publish.
            std::string dynamic = std::to_string(cnt++);

            cli.publish(deviceName + "/counter", dynamic.c_str(), dynamic.size());

            data = cameraController.MakeSinglePicture();
            cli.publish(deviceName + "/picture", &data[54], data.size() - 54);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << data.size() << std::endl;
        }

        cli.disconnect();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << " ["
                  << exc.get_reason_code() << "]" << std::endl;
        return 1;
    }

    return 0;
}
