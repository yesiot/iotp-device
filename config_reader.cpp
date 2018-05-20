//
// Created by yesiot on 13-5-18.
//

#include "config_reader.h"

namespace pt = boost::property_tree;

bool ConfigReader::readConfigFile(const std::string& fileName) {
    pt::read_json(fileName, root);
}

std::string ConfigReader::getClientName() {
    return root.get<std::string>(c_clientNameKey);
}

std::string ConfigReader::getUserName() {
    return root.get<std::string>(c_userKey);
}

std::string ConfigReader::getPassword() {
    return root.get<std::string>(c_passwordKey);
}

std::string ConfigReader::getHostAddress() {
    return root.get<std::string>(c_hostKey);
}

int ConfigReader::getPortNr() {
    return root.get<int>(c_portKey);
}
