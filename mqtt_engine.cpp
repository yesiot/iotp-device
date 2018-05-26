#include "mqtt_engine.h"

MqttEngine::MqttEngine(const std::string& deviceName, const std::string& hostAddress, int portNr) :
    m_deviceName(deviceName)
{
    std::string connectionUrl = hostAddress + ":" + std::to_string(portNr);

    m_mqttClient = std::unique_ptr<mqtt::client>(new mqtt::client(connectionUrl, deviceName));

    m_connectOptions.set_keep_alive_interval(c_keepAliveIntervalMs);
    m_connectOptions.set_clean_session(c_cleanSession);


    //Last will - message that is send by broker if connection breaks
    std::string statusStr = "DEAD";
    m_connectOptions.set_will({m_deviceName + "/status", statusStr.c_str(), statusStr.size()});
}

void MqttEngine::SetPassword(const std::string& userName, const std::string& password) {
    m_connectOptions.set_user_name(userName);
    m_connectOptions.set_password(password);
}

bool MqttEngine::Connect() {
    m_mqttClient->set_callback(*this);
    m_mqttClient->connect(m_connectOptions);
    m_mqttClient->subscribe("master/#");
    return m_mqttClient->is_connected();
}

void MqttEngine::Disconnect() {
    m_mqttClient->disconnect();
}

void MqttEngine::Publish(const std::string& topic, const void* payload, size_t payloadSize) {
    m_mqttClient->publish(m_deviceName + "/" + topic, payload, payloadSize);
}

void MqttEngine::message_arrived(mqtt::const_message_ptr msg) {
    mqtt::callback::message_arrived(msg);

    std::cout << "Message arrived" << std::endl;

    if(onNewMessage) {
        onNewMessage(msg->get_topic(), msg->get_payload().data(), msg->get_payload().size());
    }
}

void MqttEngine::connected(const mqtt::string &cause) {
    mqtt::callback::connected(cause);
    std::cout << "Connected " << cause << std::endl;

}

void MqttEngine::delivery_complete(mqtt::delivery_token_ptr tok) {
    mqtt::callback::delivery_complete(tok);
}
