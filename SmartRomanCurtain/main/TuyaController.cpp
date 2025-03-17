
#include "TuyaController.h"

namespace SmartRomanCurtain
{
    // Designed to setup configure
    void TuyaController::Initialize()
    {
        xTaskCreate(StaticDoTuyaLinkAppTask, "TuyaLink", 1024 * 6, this, 2, NULL);
    }

    // Designed to display QR
    void TuyaController::PrintQrCode(const char* productkey, const char* uuid)
    {
        TY_LOGI("https://smartapp.tuya.com/s/p?p=%s&uuid=%s&v=2.0", productkey, uuid);

        char urlbuf[255];
        sprintf(urlbuf, "https://smartapp.tuya.com/s/p?p=%s&uuid=%s&v=2.0", productkey, uuid);
        qrcode_display(urlbuf);

        TY_LOGI("(Use this URL to generate a static QR code for the Tuya APP scan code binding)");
    }

    // Designed to set callback
    void TuyaController::Set(const std::function<void(const std::string&)>& changeMotorStateCallback)
    {
        _changeMotorStateCallback = changeMotorStateCallback;
    }

    /* DP data reception processing function */
    void TuyaController::ReceiveDpEvent(tuya_iot_client_t* client, const char* json_dps)
    {
        TY_LOGD("Data point download value:%s", json_dps);

        /* Parsing json string to cJSON object */
        cJSON* dps = cJSON_Parse(json_dps);
        if (dps == NULL) {
            TY_LOGE("JSON parsing error, exit!");
            return;
        }

        /* Process dp data */
        cJSON* switch_obj = cJSON_GetObjectItem(dps, CURTAIN_SWITCH_CONTROL.c_str());
        if (switch_obj != NULL && cJSON_IsString(switch_obj)) {
            TY_LOGE("CURTAIN_SWITCH_CONTROL");

            if (strcmp(switch_obj->valuestring, "open") == 0) {
                TY_LOGE("OPEN");
                _changeMotorStateCallback("open");
            } else if (strcmp(switch_obj->valuestring, "close") == 0) {
                TY_LOGE("CLOSE");
                _changeMotorStateCallback("close");
            } else if (strcmp(switch_obj->valuestring, "stop") == 0) {
                TY_LOGE("STOP");
                _changeMotorStateCallback("stop");
            }
        }

        /* relese cJSON DPS object */
        cJSON_Delete(dps);

        /* Report the received data to synchronize the switch status. */
        tuya_iot_dp_report_json(client, json_dps);
    }

    /* Tuya SDK event callback */
    void TuyaController::HandleUserEvent(tuya_iot_client_t* client, tuya_event_msg_t* event)
    {
        switch(event->id){
        case TUYA_EVENT_BIND_START:
            PrintQrCode(client->config.productkey, client->config.uuid);
            break;

        case TUYA_EVENT_MQTT_CONNECTED:
            PrintQrCode(client->config.productkey, client->config.uuid);
            TY_LOGI("Device MQTT Connected!");
            break;

        case TUYA_EVENT_DP_RECEIVE:
            ReceiveDpEvent(client, (const char*)event->value.asString);
            break;

        default:
            break;
        }
    }

    // Designed to start task Tuya handlers
    void TuyaController::DoTuyaLinkAppTask()
    {
        int ret = OPRT_OK;

        tuya_iot_config_t tuyaIotConfig;
        tuyaIotConfig.software_ver = "1.0.0";
        tuyaIotConfig.productkey = TUYA_PRODUCT_KEY.c_str();
        tuyaIotConfig.uuid = TUYA_DEVICE_UUID.c_str();
        tuyaIotConfig.authkey = TUYA_DEVICE_AUTHKEY.c_str();
        tuyaIotConfig.storage_namespace = "tuya_kv";
        tuyaIotConfig.event_handler = StaticHandleUserEvent;
        /* Initialize Tuya device configuration */
        ret = tuya_iot_init(&_client, &tuyaIotConfig);

        _client.userData = this;

        assert(ret == OPRT_OK);

        /* Start tuya iot task */
        tuya_iot_start(&_client);

        for(;;) {
            /* Loop to receive packets, and handles client keepalive */
            tuya_iot_yield(&_client);
        }
    }

    // Designed for wrapping HandleUserEvent
    void TuyaController::StaticHandleUserEvent(tuya_iot_client_t* client, tuya_event_msg_t* event)
    {
        TuyaController* self = static_cast<TuyaController*>(client->userData);
        if (self) {
            self->HandleUserEvent(client, event);
        }
    }

    // Designed for wrapping HandleUserEvent
    void TuyaController::StaticDoTuyaLinkAppTask(void* pvParameters)
    {
        TuyaController* self = static_cast<TuyaController*>(pvParameters);
        if (self) {
            self->DoTuyaLinkAppTask();
        }
    }
}

