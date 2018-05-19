//
// Created by yesiot on 13-5-18.
//

#ifndef PAHO_TEST_CONFIG_READER_H
#define PAHO_TEST_CONFIG_READER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class ConfigReader {

    static constexpr const char * c_clientNameKey = "name";
    static constexpr const char * c_hostKey = "host";
    static constexpr const char * c_portKey = "port";
    static constexpr const char * c_userKey = "user";
    static constexpr const char * c_passwordKey = "password";

public:
    ConfigReader() = default;
    ConfigReader(const ConfigReader&) = delete;
    ConfigReader& operator=(const ConfigReader&) = delete;

    bool readConfigFile(const std::string& fileName);

    std::string getClientName();
    std::string getUserName();
    std::string getPassword();
    std::string getConnectionUrl();

private:

    boost::property_tree::ptree root;

};


#endif //PAHO_TEST_CONFIG_READER_H
