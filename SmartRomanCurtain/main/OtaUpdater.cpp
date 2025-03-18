#include "OtaUpdater.h"

namespace SmartRomanCurtain {

    // Designed to wrap method Do
    void OtaUpdater::StaticDoOtaUpdateTask(void *arg)
    {
       OtaUpdater* self = static_cast<OtaUpdater*>(arg);
       if (self) {
           self->DoOtaUpdateTask();
       }

      vTaskDelete(NULL);
    }

    void OtaUpdater::DoOtaUpdateTask()
    {
        esp_http_client_config_t config = {};

        config.url = FIRMWARE_URL;
        config.cert_pem = (const char *)YANDEX_ROOT_CA.c_str();
        config.event_handler = HandleHttpEventWrapper;
        config.user_data = this;

        _totalBytesReceived = 0;
        _otaProgress = 0;
        _isFileFound = false;
        _otaStatus = static_cast<uint32_t>(EnOtaStatus::OTA_STARTED_PERORM_LOADING);

        ESP_LOGI(TAG, "Attempting to download update from %s", config.url);
        esp_err_t ret = esp_https_ota(&config);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "OTA Succeed, Rebooting...");
            vTaskDelay(1000);
            esp_restart();
        } else {
            ESP_LOGE(TAG, "Firmware upgrade failed");
        }
    }

    // Designed to try load firmware from HTTP server
    void OtaUpdater::OtaUpdate()
    {
        xTaskCreate(&StaticDoOtaUpdateTask, "ota_update_task", 8192, this, 5, NULL);
    }

    // Designed to wrap method HandleHttpEvent
   esp_err_t OtaUpdater::HandleHttpEventWrapper(esp_http_client_event_t *evt)
   {
       OtaUpdater* self = static_cast<OtaUpdater*>(evt->user_data);
       if (self) {
           return self->HandleHttpEvent(evt);
       }

       return ESP_FAIL;
   }

    // Designed to handle HTTP event
    esp_err_t OtaUpdater::HandleHttpEvent(esp_http_client_event_t *evt)
    {
        switch (evt->event_id) {
            case HTTP_EVENT_ERROR:
                ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
                _otaStatus = static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_FAILED);
                ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
                break;
            case HTTP_EVENT_ON_CONNECTED:
                ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
                _otaStatus = static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_IN_PROGRESS);
                ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
                break;
            case HTTP_EVENT_HEADER_SENT:
                ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
                break;
            case HTTP_EVENT_ON_HEADER: {
                // Checking the Content-Disposition Header
                if (strcasecmp(evt->header_key, "Content-Disposition") == 0) {
                    if (strstr(evt->header_value, FIRMWARE_NAME.c_str()) != NULL) {
                        _isFileFound = true;
                        ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER FILE FOUND");
                    }
                }
                ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            }
                break;
            case HTTP_EVENT_ON_DATA: {
                _totalBytesReceived += evt->data_len;
                if (_isFileFound) {
                    _otaProgress = (esp_http_client_get_content_length(evt->client) > 0) ?
                                               (100 * _totalBytesReceived) / esp_http_client_get_content_length(evt->client) :
                                               0;
                }
                ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            }
                break;
            case HTTP_EVENT_ON_FINISH:
                if (_isFileFound && _otaProgress == 100) {
                    _otaStatus = static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_SUCCESS);
                } else if (!_isFileFound) {
                    _otaStatus = static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_FILE_NOT_FOUND);
                } else {
                    _otaStatus  = static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_CONNECTION_FAILED);
                }
                ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
                break;
            case HTTP_EVENT_DISCONNECTED:
                // Disconnect state handle

                if (_isFileFound && _otaProgress == 100) {
                    _otaStatus = static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_SUCCESS);
                } else if (!_isFileFound) {
                    _otaStatus = static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_FILE_NOT_FOUND);
                } else {
                    _otaStatus  = static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_CONNECTION_FAILED);
                }

                ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
                break;
        }
        return ESP_OK;
    }

    // Designed for read OTA progress state
    uint32_t OtaUpdater::GetOtaProgress()
    {
        return _otaProgress.load();
    }


    // Designed for read OTA status description
    std::string OtaUpdater::GetOtaStatusMessage()
    {
        if (_otaStatus.load() == static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_FILE_NOT_FOUND)) {
            return "Файл прошивки не найден на сервере.";
        } else if (_otaStatus.load() == static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_CONNECTION_FAILED)) {
            return "Ошибка подключения к серверу.";
        } else if (_otaStatus.load() == static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_IN_PROGRESS)) {
            return std::string("Обновление в процессе...") + "( " + std::to_string(_otaProgress) + "% )";
        } else if (_otaStatus.load() == static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_FAILED)) {
            return "Ошибка при обновлении прошивки.";
        } else if (_otaStatus.load() == static_cast<uint32_t>(EnOtaStatus::OTA_STATUS_SUCCESS)) {
            return "Прошивка успешно обновлена.";
        } else if (_otaStatus.load() == static_cast<uint32_t>(EnOtaStatus::OTA_STARTED_PERORM_LOADING)) {
            return "OTA начата. Подготовка к загрузке...";
        }

        return "Неизвестный статус";
    }


} /* namespace SmartRomanCurtain */
