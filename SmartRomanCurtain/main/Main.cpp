
#include "NvsMemoryManager.h"
#include "TuyaController.h"
#include "WebServerManager.h"
#include "MotorController.h"
#include "DeepSleepNtp.h"
#include "YandexDialogController.h"
#include "WiFiConnector.h"
#include "OtaUpdater.h"

namespace SmartRomanCurtain
{
    TuyaController _tuyaController;
    WebServerManager _webServerManager;
    NvsMemoryManager _nvsMemoryManager;
    MotorController _motorController;
    DeepSleepNtp _deepSleepNtp;
    YandexDialogController _yandexDialogController;
    WiFiConnector _wiFiConnector;
    OtaUpdater _otaUpdater;

    extern "C" void app_main(void)
    {
        ESP_LOGI("MAIN", "[APP] Startup..");
        ESP_LOGI("MAIN", "[APP] Free memory: %d bytes", esp_get_free_heap_size());
        ESP_LOGI("MAIN", "[APP] IDF version: %s", esp_get_idf_version());

        esp_log_level_set("*", ESP_LOG_INFO);
        esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
        esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
        esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
        esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

        _webServerManager.Set(&_motorController);
        _webServerManager.Set(&_nvsMemoryManager);
        _webServerManager.Set(&_deepSleepNtp);
        _motorController.Set(&_nvsMemoryManager);
        _deepSleepNtp.Set(&_nvsMemoryManager);
        _wiFiConnector.Set(&_otaUpdater);
        _webServerManager.Set([&](const int32_t workMode) { _motorController.HandleWorkModeChanged(workMode); });
        _yandexDialogController.Set([&](const std::string& motorState) { _motorController.HandleMotorStateChanged(motorState); });
        _yandexDialogController.Set([&](const std::uint32_t range) { _motorController.HandleMotorRangeChanged(range); });
        _webServerManager.SetDeviceId(_yandexDialogController.GetDeviceId());
        //_tuyaController.Set([=](const std::string& motorState) { _motorController.HandleMotorStateChanged(motorState); });

        esp_netif_init();
        esp_event_loop_create_default();

        _nvsMemoryManager.Initialize();
        _webServerManager.Initialize();
        _motorController.Initialize();
        _yandexDialogController.Initialize();
        //_tuyaController.Initialize();

        _wiFiConnector.Connect(_webServerManager.GetLogin(), _webServerManager.GetPassword());
        _webServerManager.StartWebServer();

        _deepSleepNtp.Inititalize();
    }
}
