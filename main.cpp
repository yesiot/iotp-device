#include <iostream>
#include "mqtt/async_client.h"
#include "mqtt/client.h"

#include "config_reader.h"



void showUsage() {
    std::cout << "Usage:\n"\
                 "paho_test <confiSettingValueg file name>\n\n"\
                 "config file in the json format containing following keys:\n"\
                 "host     - address of the mqtt broker\n"\
                 "port     - port number used by mqtt service\n"\
                 "user     - username used in the mqtt broker authentication\n"\
                 "password - password used for broker authentication" << std::endl;
}

int main(int argc, char* argv[])
{
    const std::string c_deviceName = "rpi0Test";

    if(argc < 1) {
        std::cerr << "Invalid number of arguments" << std::endl;
        showUsage();
        return -1;
    }

    ConfigReader configReader;
    configReader.readConfigFile(argv[1]);

    std::string userName = configReader.getUserName();
    std::string userPassword = configReader.getPassword();

    mqtt::client cli(configReader.getConnectionUrl(), c_deviceName);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    connOpts.set_user_name(userName);
    connOpts.set_password(userPassword);

    try {

        const auto qos = 0;
        const auto retained = false;

        cli.connect(connOpts);

        cli.publish(c_deviceName + "/discovery", c_deviceName.c_str(), c_deviceName.size(), qos, retained);

        auto cnt = 0;
        while(true) {

            // Now try with itemized publish.
            std::string dynamic = std::to_string(cnt);

            cli.publish(c_deviceName + "/counter", dynamic.c_str(), dynamic.size(), qos, retained);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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