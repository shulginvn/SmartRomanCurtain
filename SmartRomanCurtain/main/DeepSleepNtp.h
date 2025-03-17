#pragma once

#include <ctime>
#include <vector>
#include <string>

#include <esp_sleep.h>
#include <esp_sntp.h>
#include <esp_log.h>

#include "NvsMemoryManager.h"

namespace SmartRomanCurtain
{
    class DeepSleepNtp {
        public:

            struct StSleepModeParamas {
                int32_t enableSleep = 0;
                int32_t sleepStartHour = 0;
                int32_t sleepStartMinute = 0;
                int32_t sleepDurationHours = 0;
            };

            // Designed to initialize first inner state
            void Inititalize();

            // Designed to set NVS manager instance
            void Set(NvsMemoryManager* nvsMemoryManager);

            // Designed to set sleep mode params
            void SetSleepModeParams(const std::vector<StSleepModeParamas>& sleepModeParamsList);

            // Designed to read current time hour
            int32_t GetCurrentHour() const;

            // Designed to read current time minute
            int32_t GetCurrentMinute() const;

        private:
            const std::string TAG = "DEEP_SLEEP_NTP";
            const std::string NTP_SERVER = "pool.ntp.org";

            NvsMemoryManager* _nvsMemoryManager;

            std::vector<StSleepModeParamas> _sleepModeParamsList;

            int32_t _currentHour = 0;
            int32_t _currentMinute = 0;

            // Designed to wrap task DoTimeMonitorTask
            static void StaticDoTimeMonitorTask(void *arg);

            // Designed to time monitoring
            void DoTimeMonitorTask();
    };

}
