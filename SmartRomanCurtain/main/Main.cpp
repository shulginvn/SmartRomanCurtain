
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
    std::string CURRENT_FIRMWARE_VERSION = "1.02";

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

        _webServerManager.SetCurrentFirmwareVersion(CURRENT_FIRMWARE_VERSION);

        _webServerManager.Set(&_motorController);
        _webServerManager.Set(&_nvsMemoryManager);
        _webServerManager.Set(&_deepSleepNtp);
        _webServerManager.Set(&_otaUpdater);

        _motorController.Set(&_nvsMemoryManager);
        _deepSleepNtp.Set(&_nvsMemoryManager);
        _yandexDialogController.Set([&](const std::string& motorState) { _motorController.HandleMotorStateChanged(motorState); });
        _yandexDialogController.Set([&](const std::uint32_t range) { _motorController.HandleMotorRangeChanged(range); });
        _yandexDialogController.Set([&]() -> std::pair<bool, int32_t> { return _motorController.GetMotorOnOffAndRange(); });
        _webServerManager.Set([&](const std::string& motorState) { _motorController.HandleMotorStateChanged(motorState); });

        esp_netif_init();
        esp_event_loop_create_default();

        _nvsMemoryManager.Initialize();
        _webServerManager.Initialize();
        _motorController.Initialize();

        // Read from NVS
        int32_t uniqueId = 0;
        _nvsMemoryManager.ReadDataFromFlash("UniqueId", &uniqueId);
        // Set unique device parameters
        _webServerManager.SetDeviceId(uniqueId);

        // Read from NVS
        char bufferUsername[64] = { }, bufferPassword[64] = { };
        int32_t resMgttLog = _nvsMemoryManager.ReadDataFromFlash("MqttLog", bufferUsername);
        int32_t resMqttPwdLog = _nvsMemoryManager.ReadDataFromFlash("MqttPwd", bufferPassword);
        std::string mqttUsername = std::string(bufferUsername), mqttPassword = std::string(bufferPassword);

        // Set MQTT parameters
        if (resMgttLog == ESP_OK && resMqttPwdLog == ESP_OK && !mqttUsername.empty() && !mqttPassword.empty()) {
            _yandexDialogController.SetMqttAuthInfo(mqttUsername, mqttPassword);
            _yandexDialogController.Initialize();
        }

        _webServerManager.Set([&](const std::string& mqttUsername, const std::string& mqttPassword) {
            _yandexDialogController.Deinitialize();
            _yandexDialogController.SetMqttAuthInfo(mqttUsername, mqttPassword);
            _yandexDialogController.Initialize();
        });

        // Start NET services
        _wiFiConnector.Connect(_webServerManager.GetLogin(), _webServerManager.GetPassword());
        _webServerManager.StartWebServer();

        _deepSleepNtp.Inititalize();
    }
}
