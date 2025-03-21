
#include "DeepSleepNtp.h"

namespace SmartRomanCurtain
{
    // Designed to set NVS manager instance
    void DeepSleepNtp::Set(NvsMemoryManager* nvsMemoryManager)
    {
        _nvsMemoryManager = nvsMemoryManager;
    }

    // Designed to initialize first inner state
    void DeepSleepNtp::Inititalize()
    {
        ESP_LOGI(TAG.c_str(), "Initializing NTP");
        sntp_setoperatingmode(SNTP_OPMODE_POLL); // @suppress("Invalid arguments")
        sntp_setservername(0, NTP_SERVER.c_str()); // @suppress("Invalid arguments")
        sntp_init();

        //Wait sync time
        int retry = 0;
        const int retry_count = 30;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
            ESP_LOGI(TAG.c_str(), "Waiting for system time to be set... (%d/%d)", retry, retry_count);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if (retry == retry_count) {
            ESP_LOGE(TAG.c_str(), "Failed to synchronize time with NTP server!");
        } else {
            // Getting current time
            time_t now;
            time(&now);
            ESP_LOGI(TAG.c_str(), "Current time: %s", ctime(&now));
        }

        xTaskCreate(StaticDoTimeMonitorTask, "EncoderTask", 4096, this, 1, NULL);
    }

    // Designed to wrap task DoTimeMonitorTask
    void DeepSleepNtp::StaticDoTimeMonitorTask(void *arg)
    {
        DeepSleepNtp* self = static_cast<DeepSleepNtp*>(arg);
        if (self) {
            self->DoTimeMonitorTask();
        }
    }

    // Designed to time monitoring
    void DeepSleepNtp::DoTimeMonitorTask()
    {
        int32_t sleepModeParamsQty = 0;
        _nvsMemoryManager->ReadDataFromFlash("smpq", &sleepModeParamsQty);
        _sleepModeParamsList.resize(sleepModeParamsQty);

        for (int32_t i = 0; i < _sleepModeParamsList.size(); i++) {
            _nvsMemoryManager->ReadDataFromFlash(("es" + std::to_string(i)).c_str(), &_sleepModeParamsList.at(i).enableSleep);
            _nvsMemoryManager->ReadDataFromFlash(("ssh" + std::to_string(i)).c_str(), &_sleepModeParamsList.at(i).sleepStartHour);
            _nvsMemoryManager->ReadDataFromFlash(("ssm" + std::to_string(i)).c_str(), &_sleepModeParamsList.at(i).sleepStartMinute);
            _nvsMemoryManager->ReadDataFromFlash(("sdh" + std::to_string(i)).c_str(), &_sleepModeParamsList.at(i).sleepDurationHours);
        }

        while (true) {
            time_t now;
            time(&now);
            struct tm timeinfo;
            localtime_r(&now, &timeinfo);

            _currentHour = timeinfo.tm_hour;
            _currentMinute = timeinfo.tm_min;

            ESP_LOGI(TAG.c_str(), "Current time: %02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);

            // Check if we have reached target time
            for (auto& sleepModeParams : _sleepModeParamsList) {
                if (sleepModeParams.enableSleep && timeinfo.tm_hour == sleepModeParams.sleepStartHour && timeinfo.tm_min == sleepModeParams.sleepStartMinute) {
                    ESP_LOGI(TAG.c_str(), "Target time reached! Going to deep sleep...");

                    // Calculate wake up time
                    // time_t wake_time = now + (_sleepDurationHours * 3600);
                    // ESP_LOGI(TAG.c_str(), "Next wakeup scheduled at %s", asctime(localtime(&wake_time)));

                    int64_t sleep_duration_us = (sleepModeParams.sleepDurationHours * 3600) * 1000000LL;

                    // Set a wake-up timer and go into deep sleep
                    esp_sleep_enable_timer_wakeup(sleep_duration_us);
                    esp_deep_sleep_start();
                }
            }

            // Pause for 60 seconds before the next check
            vTaskDelay(pdMS_TO_TICKS(60000));
        }
    }

    // Designed to set sleep mode params
    void DeepSleepNtp::SetSleepModeParams(const std::vector<StSleepModeParamas>& sleepModeParamsList)
    {
        _sleepModeParamsList = std::move(sleepModeParamsList);
        _nvsMemoryManager->WriteDataToFlash("smpq", _sleepModeParamsList.size());

        for (int32_t i = 0; i < _sleepModeParamsList.size(); i++) {
            _nvsMemoryManager->WriteDataToFlash(("es" + std::to_string(i)).c_str(), _sleepModeParamsList.at(i).enableSleep);
            _nvsMemoryManager->WriteDataToFlash(("ssh" + std::to_string(i)).c_str(), _sleepModeParamsList.at(i).sleepStartHour);
            _nvsMemoryManager->WriteDataToFlash(("ssm" + std::to_string(i)).c_str(), _sleepModeParamsList.at(i).sleepStartMinute);
            _nvsMemoryManager->WriteDataToFlash(("sdh" + std::to_string(i)).c_str(), _sleepModeParamsList.at(i).sleepDurationHours);
        }
    }

    // Designed to read current time hour
    int32_t DeepSleepNtp::GetCurrentHour() const
    {
        return _currentHour;
    }

    // Designed to read current time minute
    int32_t DeepSleepNtp::GetCurrentMinute() const
    {
        return _currentMinute;
    }
}

