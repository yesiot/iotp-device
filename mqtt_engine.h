#ifndef PAHO_TEST_MQTT_ENGINE_H
#define PAHO_TEST_MQTT_ENGINE_H

#include "mqtt/async_client.h"
#include "mqtt/client.h"
#include <memory>
#include <functional>

class MqttEngine : public mqtt::callback {
    static const int  c_keepAliveIntervalMs = 20;
    static const bool c_cleanSession = true;

public:
    MqttEngine(const std::string& deviceName, const std::string& hostAddress, int portNr);

    void SetPassword(const std::string& userName, const std::string& password);
    bool Connect();
    void Disconnect();

    void Publish(const std::string& topic, const void* payload, size_t payloadSize);

    void message_arrived(mqtt::const_message_ptr msg) override;

    void connected(const mqtt::string &cause) override;

    void delivery_complete(mqtt::delivery_token_ptr tok) override;

    std::function<void(const std::string&, const void*, size_t)> onNewMessage = nullptr;

private:
    std::unique_ptr<mqtt::client> m_mqttClient;
    mqtt::connect_options         m_connectOptions;

    const std::string             m_deviceName;
};


#endif //PAHO_TEST_MQTT_ENGINE_H
