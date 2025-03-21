
#include "YandexDialogController.h"

namespace SmartRomanCurtain {

    extern const uint8_t rootCA_crt_start[] asm("_binary_rootCA_crt_start");
    extern const uint8_t rootCA_crt_end[] asm("_binary_rootCA_crt_end"); // @suppress("Unused variable declaration in file scope")

    YandexDialogController::YandexDialogController() {
        // TODO Auto-generated constructor stub
        _client = {};
    }

    YandexDialogController::~YandexDialogController() {
        // TODO Auto-generated destructor stub
    }

    // Designed to wrap task HandleMqttEvent
    void YandexDialogController::StaticHandleMqttEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
    {
        YandexDialogController* self = static_cast<YandexDialogController*>(handler_args);
        if (self) {
            self->HandleMqttEvent(handler_args, base, event_id, event_data);
        }
    }

    // Designed to handle mqtt event by differ cases
    void YandexDialogController::HandleMqttEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
    {
        esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;

        switch (static_cast<esp_mqtt_event_id_t>(event_id)) {
            case MQTT_EVENT_CONNECTED: {
                    ESP_LOGI(TAG.c_str(), "MQTT connected");
                    const std::string MQTT_COMMANDS_TOPIC = "$devices/" + _mqttUsername + "/commands";
                    esp_mqtt_client_subscribe(_client, MQTT_COMMANDS_TOPIC.c_str(), 0);
                }
                break;

            case MQTT_EVENT_DISCONNECTED:
                ESP_LOGI(TAG.c_str(), "MQTT disconnected");
                break;

            case MQTT_EVENT_DATA: {
                    ESP_LOGI(TAG.c_str(), "Data is received:");
                    ESP_LOGI(TAG.c_str(), "Topic: %.*s", event->topic_len, event->topic);
                    ESP_LOGI(TAG.c_str(), "Data: %.*s", event->data_len, event->data);

                    if (strstr(event->data, "open") != NULL) {
                        _changeMotorStateCallback("open");
                        ESP_LOGI(TAG.c_str(), "Command: open curtain");

                    } else if (strstr(event->data, "close") != NULL) {
                        _changeMotorStateCallback("close");
                        ESP_LOGI(TAG.c_str(), "Command: close curtain");
                    } else {
                        uint32_t value = std::stoi(std::string(event->data, event->data_len));
                        ESP_LOGI(TAG.c_str(), "Value: %d", value);
                        _changeMotorRangeCallback(value);
                    }
                    // Form JSON-response
                    std::string response = R"({"status": ")" + std::string("DONE") + R"("})";

                    const std::string MQTT_EVENTS_TOPIC = "$devices/" + _mqttUsername + "/events";
                    // Sending a response to the event topic
                    esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                    ESP_LOGI(TAG.c_str(), "Sent state response: %s", response.c_str());
                }
                break;

            case MQTT_EVENT_ERROR:
                ESP_LOGI(TAG.c_str(), "MQTT Error");
                break;

            default:
                break;
        }
    }

    // Designed to set authenticate information
    void YandexDialogController::SetMqttAuthInfo(const std::string mqttUsername, const std::string mqttPassword)
    {
        _mqttUsername = mqttUsername;
        _mqttPassword = mqttPassword;
    }

    // Designed to setup configure
    void YandexDialogController::Initialize()
    {
        esp_mqtt_client_config_t mqttCfg = {};
        mqttCfg.uri = MQTT_BROKER.c_str();
        mqttCfg.port = MQTT_PORT;
        mqttCfg.username = _mqttUsername.c_str();
        mqttCfg.password =  _mqttPassword.c_str();
        mqttCfg.cert_pem = (const char *)rootCA_crt_start;

        _client = esp_mqtt_client_init(&mqttCfg);
        ESP_LOGI(TAG.c_str(), "esp_mqtt_client_init");
        esp_mqtt_client_register_event(_client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, &StaticHandleMqttEvent, this);
        ESP_LOGI(TAG.c_str(), "esp_mqtt_client_register_event");
        esp_mqtt_client_start(_client);
        ESP_LOGI(TAG.c_str(), "esp_mqtt_client_start");
    }

    // Designed to reset configure and stop
    void YandexDialogController::Deinitialize()
    {
        if (_client != nullptr) {
            ESP_LOGI(TAG.c_str(), "Disconnecting MQTT client");
            esp_mqtt_client_disconnect(_client);
            ESP_LOGI(TAG.c_str(), "Stopping MQTT client");
            esp_mqtt_client_stop(_client);
            ESP_LOGI(TAG.c_str(), "Destroying MQTT client");
            esp_mqtt_client_destroy(_client);
            _client = nullptr;
        }

        ESP_LOGI(TAG.c_str(), "MQTT client deinitialized");
    }

    // Designed to set callback
    void YandexDialogController::Set(const std::function<void(const std::string&)>& changeMotorStateCallback)
    {
        _changeMotorStateCallback = changeMotorStateCallback;
    }

    // Designed to set callback
    void YandexDialogController::Set(const std::function<void(const std::uint32_t)>& changeMotorRangeCallback)
    {
        _changeMotorRangeCallback = changeMotorRangeCallback;
    }

} /* namespace SmartRomanCurtain */
