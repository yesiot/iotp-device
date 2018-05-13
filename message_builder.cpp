//
// Created by yesiot on 13-5-18.
//

#include "message_builder.h"
#include <sstream>
#include <iostream>

std::string MessageBuilder::configMessage(const std::string& deviceName) {

    std::stringstream result;

    // Short alias for this namespace
    namespace pt = boost::property_tree;

    // Create a root
    pt::ptree root;

    root.put("height", 10);
    root.put("some.complex.path", "bonjour");

    pt::write_json(result, root);

    std::cerr << result.str() << std::endl;

    return result.str();
}