//
// Created by yesiot on 13-5-18.
//

#ifndef PAHO_TEST_MESSAGEBUILDER_H
#define PAHO_TEST_MESSAGEBUILDER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class MessageBuilder {

public:
    static std::string configMessage(const std::string& deviceName);

};


#endif //PAHO_TEST_MESSAGEBUILDER_H
