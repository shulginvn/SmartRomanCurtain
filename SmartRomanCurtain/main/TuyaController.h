#pragma once

#include "tuya_log.h"
#include "tuya_iot.h"
#include "cJSON.h"
#include "qrcode.h"

#include "esp_system.h"
#include "esp_event.h"

#include <string>
#include <functional>

namespace SmartRomanCurtain
{
    class TuyaController {
        public:

            // Designed to setup configure
            void Initialize();

            // Designed to set callback
            void Set(const std::function<void(const std::string&)>& changeMotorStateCallback);

        private:
            const std::string CURTAIN_SWITCH_CONTROL = "1";
            const std::string TUYA_PRODUCT_KEY = "x6p7jscyporyhnqg";
            const std::string TUYA_DEVICE_UUID = "uuid3f75b4359e45d781";/*"uuid99e03e82c0f15114"*/;
            const std::string TUYA_DEVICE_AUTHKEY = "9VgUX5OiCWE1Gz9NhqNxRS44mL0VjfDk";/*"qmCGkXOBaEbLAiRahCzCPhbz2UNVmlNz"*/;

            /* Tuya device handle */
            tuya_iot_client_t _client;

            std::function<void(const std::string&)> _changeMotorStateCallback;

            // Designed to start task Tuya handlers
            void DoTuyaLinkAppTask();

            // Designed to display QR
            void PrintQrCode(const char* productkey, const char* uuid);

            // Designed to DP data reception processing function */
            void ReceiveDpEvent(tuya_iot_client_t* client, const char* json_dps);

            // Designed to  Tuya SDK event callback
            void HandleUserEvent(tuya_iot_client_t* client, tuya_event_msg_t* event);

            // Designed for wrapping HandleUserEvent
            static void StaticHandleUserEvent(tuya_iot_client_t* client, tuya_event_msg_t* event);

            // Designed for wrapping TuyaLinkAppTask
            static void StaticDoTuyaLinkAppTask(void* pvParameters);
    };
}
