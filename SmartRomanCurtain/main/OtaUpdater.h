#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "esp_task_wdt.h"

#include <string>
#include <atomic>

namespace SmartRomanCurtain {

    class OtaUpdater {
        public:
            // Designed to try load firmware from HTTP server
            void OtaUpdate();

            // Designed for read OTA progress state
            uint32_t GetOtaProgress();

            // Designed for read OTA status description
            std::string GetOtaStatusMessage();

        private:

            enum class EnOtaStatus {
                OTA_STATUS_FILE_NOT_FOUND = 0,
                OTA_STATUS_CONNECTION_FAILED = 1,
                OTA_STATUS_IN_PROGRESS = 2,
                OTA_STATUS_FAILED = 3,
                OTA_STATUS_SUCCESS = 4,
                OTA_STARTED_PERORM_LOADING = 5
            };
           const char* TAG = "OTA_UPDATE";
           const char* FIRMWARE_URL = "https://functions.yandexcloud.net/d4eks3cvsg1jd1ev016f?firmware=SmartRomanCurtain.bin";
           const std::string YANDEX_ROOT_CA = "-----BEGIN CERTIFICATE-----\nMIIETjCCAzagAwIBAgINAe5fFp3/lzUrZGXWajANBgkqhkiG9w0BAQsFADBXMQsw\nCQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEQMA4GA1UECxMH\nUm9vdCBDQTEbMBkGA1UEAxMSR2xvYmFsU2lnbiBSb290IENBMB4XDTE4MDkxOTAw\nMDAwMFoXDTI4MDEyODEyMDAwMFowTDEgMB4GA1UECxMXR2xvYmFsU2lnbiBSb290\nIENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNpZ24xEzARBgNVBAMTCkdsb2JhbFNp\nZ24wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDMJXaQeQZ4Ihb1wIO2\nhMoonv0FdhHFrYhy/EYCQ8eyip0EXyTLLkvhYIJG4VKrDIFHcGzdZNHr9SyjD4I9\nDCuul9e2FIYQebs7E4B3jAjhSdJqYi8fXvqWaN+JJ5U4nwbXPsnLJlkNc96wyOkm\nDoMVxu9bi9IEYMpJpij2aTv2y8gokeWdimFXN6x0FNx04Druci8unPvQu7/1PQDh\nBjPogiuuU6Y6FnOM3UEOIDrAtKeh6bJPkC4yYOlXy7kEkmho5TgmYHWyn3f/kRTv\nriBJ/K1AFUjRAjFhGV64l++td7dkmnq/X8ET75ti+w1s4FRpFqkD2m7pg5NxdsZp\nhYIXAgMBAAGjggEiMIIBHjAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB\n/zAdBgNVHQ4EFgQUj/BLf6guRSSuTVD6Y5qL3uLdG7wwHwYDVR0jBBgwFoAUYHtm\nGkUNl8qJUC99BM00qP/8/UswPQYIKwYBBQUHAQEEMTAvMC0GCCsGAQUFBzABhiFo\ndHRwOi8vb2NzcC5nbG9iYWxzaWduLmNvbS9yb290cjEwMwYDVR0fBCwwKjAooCag\nJIYiaHR0cDovL2NybC5nbG9iYWxzaWduLmNvbS9yb290LmNybDBHBgNVHSAEQDA+\nMDwGBFUdIAAwNDAyBggrBgEFBQcCARYmaHR0cHM6Ly93d3cuZ2xvYmFsc2lnbi5j\nb20vcmVwb3NpdG9yeS8wDQYJKoZIhvcNAQELBQADggEBACNw6c/ivvVZrpRCb8RD\nM6rNPzq5ZBfyYgZLSPFAiAYXof6r0V88xjPy847dHx0+zBpgmYILrMf8fpqHKqV9\nD6ZX7qw7aoXW3r1AY/itpsiIsBL89kHfDwmXHjjqU5++BfQ+6tOfUBJ2vgmLwgtI\nfR4uUfaNU9OrH0Abio7tfftPeVZwXwzTjhuzp3ANNyuXlava4BJrHEDOxcd+7cJi\nWOx37XMiwor1hkOIreoTbv3Y/kIvuX1erRjvlJDKPSerJpSZdcfL03v3ykzTr1Eh\nkluEfSufFT90y1HonoMOFm8b50bOI7355KKL0jlrqnkckSziYSQtjipIcJDEHsXo\n4HA=\n-----END CERTIFICATE-----";

           std::atomic<uint32_t> _totalBytesReceived;
           std::atomic<uint32_t> _otaProgress;
           std::atomic<uint32_t> _otaStatus;
           bool _isFileFound = false;

           const std::string FIRMWARE_NAME = "SmartRomanCurtain.bin";

           // Designed to wrap method Do
           static void StaticDoOtaUpdateTask(void *arg);

           // Designed to wrap method HandleHttpEvent
           static esp_err_t HandleHttpEventWrapper(esp_http_client_event_t *evt);

           // Designed to handle HTTP event
           esp_err_t HandleHttpEvent(esp_http_client_event_t *evt);

           // Designed to run all update proccess
           void DoOtaUpdateTask();
    };

} /* namespace SmartRomanCurtain */

