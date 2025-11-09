
#include "YandexDialogController.h"

namespace SmartRomanCurtain {

    extern const uint8_t rootCA_crt_start[] asm("_binary_rootCA_crt_start");
    extern const uint8_t rootCA_crt_end[] asm("_binary_rootCA_crt_end"); // @suppress("Unused variable declaration in file scope")

    YandexDialogController::YandexDialogController() {
        // TODO Auto-generated constructor stub
        _client = {};
        _heartbeatTimer = nullptr;
    }

    YandexDialogController::~YandexDialogController() {
        // TODO Auto-generated destructor stub
        if (_heartbeatTimer != nullptr) {
            xTimerDelete(_heartbeatTimer, portMAX_DELAY);
        }
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
                    StartHeartbeatTimer();
                }
                break;

            case MQTT_EVENT_DISCONNECTED: {
                    ESP_LOGI(TAG.c_str(), "MQTT disconnected");
                    const std::string MQTT_COMMANDS_TOPIC = "$devices/" + _mqttUsername + "/commands";
                    esp_mqtt_client_unsubscribe(_client, MQTT_COMMANDS_TOPIC.c_str());
                    StopHeartbeatTimer();
                }
                break;

            case MQTT_EVENT_DATA: {
                    ESP_LOGI(TAG.c_str(), "Data is received:");
                    ESP_LOGI(TAG.c_str(), "Topic: %.*s", event->topic_len, event->topic);
                    ESP_LOGI(TAG.c_str(), "Data: %.*s", event->data_len, event->data);

                    const std::string MQTT_EVENTS_TOPIC = "$devices/" + _mqttUsername + "/events";
                    std::string response = {};

                    cJSON *root = cJSON_Parse(event->data);
                    if (root != NULL) {
                        cJSON *requestTypeJsonItem = cJSON_GetObjectItem(root, "request_type");
                        cJSON *valueJsonItem = cJSON_GetObjectItem(root, "value");
                        cJSON *requestIdJsonItem = cJSON_GetObjectItem(root, "request_id");
                        std::string requestId = "";
                        if (requestIdJsonItem != nullptr && cJSON_IsString(requestIdJsonItem)) {
                            requestId = requestIdJsonItem->valuestring;
                            ESP_LOGI(TAG.c_str(), "Request ID: %s", requestId.c_str());
                        }

                        if (requestTypeJsonItem != nullptr && cJSON_IsString(requestTypeJsonItem)) {
                            std::string requestType = requestTypeJsonItem->valuestring;
                            ESP_LOGI(TAG.c_str(), "Request type: %s", requestType.c_str());

                            if (requestType == "on_off") {

                                if (valueJsonItem != nullptr && cJSON_IsString(valueJsonItem)) {
                                    std::string command = valueJsonItem->valuestring;
                                    ESP_LOGI(TAG.c_str(), "On/Off command: %s", command.c_str());

                                    if (command == "open" || command == "close") {
                                        _changeMotorStateCallback(command);
                                        response = R"({"status": "DONE", "request_id": ")" + requestId + R"("})";
                                        esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                                    } else {
                                        ESP_LOGE(TAG.c_str(), "Invalid on/off command: %s", command.c_str());
                                        response = R"({"status": "ERROR", "error_message": "Invalid command"})";
                                        esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                                    }
                                } else {
                                    ESP_LOGE(TAG.c_str(), "Invalid value for on_off request");
                                    response = R"({"status": "ERROR", "error_message": "Invalid value"})";
                                    esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                                }
                            } else if (requestType == "range") {

                                if (valueJsonItem != nullptr && cJSON_IsNumber(valueJsonItem)) {
                                    uint32_t value = valueJsonItem->valueint;
                                    ESP_LOGI(TAG.c_str(), "Range value: %d", value);
                                    _changeMotorRangeCallback(value);
                                    response = R"({"status": "DONE", "request_id": ")" + requestId + R"("})";
                                    esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                                } else {
                                    ESP_LOGE(TAG.c_str(), "Invalid value for range request");
                                    response = R"({"status": "ERROR", "error_message": "Invalid value"})";
                                    esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                                }
                            } else if (requestType == "query") {

                                auto motorOnOffAndRange = _getMotorOnOffAndRangeCallback();
                                response = R"({"capabilities": [{)"
                                    R"("type": "devices.capabilities.range",)"
                                    R"("state": {)"
                                        R"("instance": "open",)"
                                        R"("value": )" + std::to_string(motorOnOffAndRange.second) + R"(})"
                                    R"(}, {)"
                                        R"("type": "devices.capabilities.on_off",)"
                                        R"("state": {)"
                                            R"("instance": "on",)"
                                            R"("value": )" + (motorOnOffAndRange.first ? "true" : "false") + R"(})"
                                    R"(}], "request_id": ")" + requestId + R"("})";
                                esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                            } else {
                                ESP_LOGE(TAG.c_str(), "Unknown request type: %s", requestType.c_str());
                                response = R"({"status": "ERROR", "error_message": "Unknown request type"})";
                                esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                            }
                        } else {
                            ESP_LOGE(TAG.c_str(), "Missing or invalid request_type field");
                            response = R"({"status": "ERROR", "error_message": "Missing request_type"})";
                            esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                        }

                        cJSON_Delete(root);
                    } else {
                        ESP_LOGE(TAG.c_str(), "JSON parse failed for data: %s", event->data);
                        response = R"({"status": "ERROR", "error_message": "Invalid JSON"})";
                        esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(), response.c_str(), response.length(), 0, 0);
                    }
                }
                break;

            case MQTT_EVENT_ERROR:
                StopHeartbeatTimer();
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
        StopHeartbeatTimer();

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

    // Designed to set callback
    void YandexDialogController::Set(const std::function<std::pair<bool, int32_t>()>& getMotorOnOffAndRangeCallback)
    {
        _getMotorOnOffAndRangeCallback = getMotorOnOffAndRangeCallback;
    }

    // Callback для таймера heartbeat
    void YandexDialogController::StaticHeartbeatTimerCallback(TimerHandle_t xTimer)
    {
        YandexDialogController* self = static_cast<YandexDialogController*>(pvTimerGetTimerID(xTimer));
        if (self) {
            self->SendHeartbeat();
        }
    }

    // Designed to send MQTT message with heartbeat info
    void YandexDialogController::SendHeartbeat()
    {
        if (/*!_isConnected ||*/ _client == nullptr) {
            ESP_LOGI(TAG.c_str(), "MQTT not connected, skipping heartbeat");
            return;
        }

        // Form JSON for heartbeat
        std::string heartbeatJson = R"({
            "device_id": ")" + _mqttUsername + R"(",
            "timestamp": )" + std::to_string(esp_timer_get_time() / 1000000) + R"(,
            "type": "heartbeat"
        })";

        const std::string MQTT_EVENTS_TOPIC = "$devices/" + _mqttUsername + "/events";

        int32_t msgId = esp_mqtt_client_publish(_client, MQTT_EVENTS_TOPIC.c_str(),
                                           heartbeatJson.c_str(), heartbeatJson.length(),
                                           0, 0);

        if (msgId == -1) {
            ESP_LOGE(TAG.c_str(), "Failed to publish heartbeat");
        } else {
            ESP_LOGI(TAG.c_str(), "Heartbeat sent: %s", heartbeatJson.c_str());
        }
    }

    // Designed to start heartbeat timer
    void YandexDialogController::StartHeartbeatTimer()
    {
        if (_heartbeatTimer == nullptr) {
            _heartbeatTimer = xTimerCreate(
                "HeartbeatTimer",
                pdMS_TO_TICKS(5000),
                pdTRUE,
                this,
                StaticHeartbeatTimerCallback
            );
        }

        if (_heartbeatTimer != nullptr) {
            if (xTimerStart(_heartbeatTimer, 0) != pdPASS) {
                ESP_LOGE(TAG.c_str(), "Failed to start heartbeat timer");
            } else {
                ESP_LOGI(TAG.c_str(), "Heartbeat timer started (5s interval)");
            }
        }
    }

    // Designed to stop heartbeat timer
    void YandexDialogController::StopHeartbeatTimer()
    {
        if (_heartbeatTimer != nullptr) {
            xTimerStop(_heartbeatTimer, 0);
            ESP_LOGI(TAG.c_str(), "Heartbeat timer stopped");
        }
    }


} /* namespace SmartRomanCurtain */
