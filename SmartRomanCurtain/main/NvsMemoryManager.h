#pragma once

#include <stdio.h>
#include <string>

#include "esp_log.h"
#include "nvs_flash.h"

namespace SmartRomanCurtain
{
    class NvsMemoryManager
    {
        public:

            // Designed to setup configure
            void Initialize();

            // Designed to store data in flash memory
            void WriteDataToFlash(const char* key, int32_t value);

            // Designed to read data from flash memory
            void ReadDataFromFlash(const char* key, int32_t* value);

            // Designed to store data from flash memory
            void WriteDataToFlash(const char* key, const char* value);

            // Designed to read data from flash memory
            int32_t ReadDataFromFlash(const char* key, const char* value);

        private:
            const std::string TAG = "NVS_MEMORY_MANAGER";
    };
}
