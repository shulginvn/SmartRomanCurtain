#pragma once

#include "mqtt_client.h"

#include "esp_log.h"

#include <string>
#include <functional>

#include "freertos/timers.h"

#include "cJSON.h"

namespace SmartRomanCurtain
{
    class YandexDialogController {
        public:
            YandexDialogController();
            virtual ~YandexDialogController();

            // Designed to setup configure
            void Initialize();

            // Designed to set authenticate information
            void SetMqttAuthInfo(const std::string mqttUsername, const std::string mqttPassword);

            // Designed to set callback
            void Set(const std::function<void(const std::string&)>& changeMotorStateCallback);

            // Designed to set callback
            void Set(const std::function<void(const std::uint32_t)>& changeMotorRangeCallback);

            // Designed to set callback
            void Set(const std::function<std::pair<bool, int32_t>()>& getMotorOnOffAndRangeCallback);

            // Designed to reset configure and stop
            void Deinitialize();

        private:
            const std::string TAG = "MQTT_EXAMPLE";

            const std::string MQTT_BROKER = "mqtts://mqtt.cloud.yandex.net";
            const uint32_t MQTT_PORT = 8883;

            std::string _mqttUsername = {};
            std::string _mqttPassword = {};

            esp_mqtt_client_handle_t _client = nullptr;

            std::function<void(const std::string&)> _changeMotorStateCallback;
            std::function<void(const std::uint32_t)> _changeMotorRangeCallback;
            std::function<std::pair<bool, int32_t>()> _getMotorOnOffAndRangeCallback;

            TimerHandle_t _heartbeatTimer;

            // Designed to wrap task DoHallSensorTask
            static void StaticHandleMqttEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

            // Designed to handle mqtt event by differ cases
            void HandleMqttEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

            // Designed to wrap call SendHeartbeat method from instance
            static void StaticHeartbeatTimerCallback(TimerHandle_t xTimer);

            // Designed to send MQTT message with heartbeat info
            void SendHeartbeat();

            // Designed to start heartbeat timer
            void StartHeartbeatTimer();

            // Designed to stop heartbeat timer
            void StopHeartbeatTimer();
    };

} /* namespace SmartRomanCurtain */

