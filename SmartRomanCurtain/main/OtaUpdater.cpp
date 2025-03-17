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

        ESP_LOGI(TAG, "Attempting to download update from %s", config.url);
        esp_err_t ret = esp_https_ota(&config);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "OTA Succeed, Rebooting...");
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
                ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
                break;
            case HTTP_EVENT_ON_CONNECTED:
                ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
                break;
            case HTTP_EVENT_HEADER_SENT:
                ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
                break;
            case HTTP_EVENT_ON_HEADER:
                ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
                break;
            case HTTP_EVENT_ON_DATA:
                ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
                break;
            case HTTP_EVENT_ON_FINISH:
                ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
                break;
            case HTTP_EVENT_DISCONNECTED:
                ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
                break;
        }
        return ESP_OK;
    }

} /* namespace SmartRomanCurtain */
