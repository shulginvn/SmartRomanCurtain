#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <functional>

#include "esp_http_server.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_wifi.h"

#include <string>
#include <vector>

#include "DeepSleepNtp.h"
#include "NvsMemoryManager.h"
#include "MotorController.h"
#include "OtaUpdater.h"

#include "cJSON.h"

namespace SmartRomanCurtain
{
    class WebServerManager
    {
        public:

            // Designed to setup configure
            void Initialize();

            // Designed to set instance
            void Set(NvsMemoryManager* nvsMemoryManager);

            // Designed to set instance
            void Set(MotorController* motorController);

            // Designed to set instance
            void Set(DeepSleepNtp* motorController);

            // Designed to set instance
            void Set(OtaUpdater* otaUpdater);

            // Designed to run HTTP server
            httpd_handle_t StartWebServer(void);

            // Designed to get login
            const char* GetLogin();

            // Designed to get password
            const char* GetPassword();

            // Designed to save device id
            void SetDeviceId(std::string deviceId);

            // Designed for set current firmware version
            void SetCurrentFirmwareVersion(const std::string& currentFirmwareVersion);

        private:

            const std::string YANDEX_ROOT_CA = "-----BEGIN CERTIFICATE-----\nMIIETjCCAzagAwIBAgINAe5fFp3/lzUrZGXWajANBgkqhkiG9w0BAQsFADBXMQsw\nCQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEQMA4GA1UECxMH\nUm9vdCBDQTEbMBkGA1UEAxMSR2xvYmFsU2lnbiBSb290IENBMB4XDTE4MDkxOTAw\nMDAwMFoXDTI4MDEyODEyMDAwMFowTDEgMB4GA1UECxMXR2xvYmFsU2lnbiBSb290\nIENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNpZ24xEzARBgNVBAMTCkdsb2JhbFNp\nZ24wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDMJXaQeQZ4Ihb1wIO2\nhMoonv0FdhHFrYhy/EYCQ8eyip0EXyTLLkvhYIJG4VKrDIFHcGzdZNHr9SyjD4I9\nDCuul9e2FIYQebs7E4B3jAjhSdJqYi8fXvqWaN+JJ5U4nwbXPsnLJlkNc96wyOkm\nDoMVxu9bi9IEYMpJpij2aTv2y8gokeWdimFXN6x0FNx04Druci8unPvQu7/1PQDh\nBjPogiuuU6Y6FnOM3UEOIDrAtKeh6bJPkC4yYOlXy7kEkmho5TgmYHWyn3f/kRTv\nriBJ/K1AFUjRAjFhGV64l++td7dkmnq/X8ET75ti+w1s4FRpFqkD2m7pg5NxdsZp\nhYIXAgMBAAGjggEiMIIBHjAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB\n/zAdBgNVHQ4EFgQUj/BLf6guRSSuTVD6Y5qL3uLdG7wwHwYDVR0jBBgwFoAUYHtm\nGkUNl8qJUC99BM00qP/8/UswPQYIKwYBBQUHAQEEMTAvMC0GCCsGAQUFBzABhiFo\ndHRwOi8vb2NzcC5nbG9iYWxzaWduLmNvbS9yb290cjEwMwYDVR0fBCwwKjAooCag\nJIYiaHR0cDovL2NybC5nbG9iYWxzaWduLmNvbS9yb290LmNybDBHBgNVHSAEQDA+\nMDwGBFUdIAAwNDAyBggrBgEFBQcCARYmaHR0cHM6Ly93d3cuZ2xvYmFsc2lnbi5j\nb20vcmVwb3NpdG9yeS8wDQYJKoZIhvcNAQELBQADggEBACNw6c/ivvVZrpRCb8RD\nM6rNPzq5ZBfyYgZLSPFAiAYXof6r0V88xjPy847dHx0+zBpgmYILrMf8fpqHKqV9\nD6ZX7qw7aoXW3r1AY/itpsiIsBL89kHfDwmXHjjqU5++BfQ+6tOfUBJ2vgmLwgtI\nfR4uUfaNU9OrH0Abio7tfftPeVZwXwzTjhuzp3ANNyuXlava4BJrHEDOxcd+7cJi\nWOx37XMiwor1hkOIreoTbv3Y/kIvuX1erRjvlJDKPSerJpSZdcfL03v3ykzTr1Eh\nkluEfSufFT90y1HonoMOFm8b50bOI7355KKL0jlrqnkckSziYSQtjipIcJDEHsXo\n4HA=\n-----END CERTIFICATE-----";
            const char* SERVER_URL = "https://functions.yandexcloud.net/d4eks3cvsg1jd1ev016f";

            static const char* _htmlForm;
            const std::string TAG = "HTTP_SERVER";

            char _login[32] = {0};
            char _password[64] = {0};

            NvsMemoryManager* _nvsMemoryManager;
            MotorController* _motorController;
            DeepSleepNtp* _deepSleepNtp;
            OtaUpdater* _otaUpdater;

            std::string _deviceId;
            std::string _jsonFirmwareVersion = {};
            std::string _currentFirmwareVersion = {};

            std::function<void(const int32_t)> _changeWorkModeCallback;

            // Designed for the compare firmware versions
            std::string CompareFirmwareVersions(const std::string& currentVersion, const std::string& serverVersion);

            // Designed to handle HTTP event callback
            static esp_err_t ProcessJsonFirmwareVersion(esp_http_client_event_t *evt);

            // Designed to set JSON firmware version
            esp_err_t SetJsonFirmwareVersion(const char* data, const uint32_t length);

            // Designed for send HTTP response with message
            esp_err_t SendHttpResponseWithMessage(httpd_req_t* req, const std::string& message);

            // Designed to handle HTTP request
            esp_err_t RootHandler(httpd_req_t* req);

            // Designed to handle HTTP request
            esp_err_t SetWiFiHandler(httpd_req_t *req);

            // Designed to handle HTTP request
            esp_err_t CalibrateHandler(httpd_req_t *req);

            // Designed to handle HTTP request
            esp_err_t AdjustPulsesHandler(httpd_req_t *req);

            // Designed to handle HTTP request
            esp_err_t SetSleepModeHandler(httpd_req_t *req);

            // Designed to handle HTTP request
            esp_err_t GetCurrentTimeHandler(httpd_req_t *req);

            // Designed to handle HTTP request
            esp_err_t SetEmailHandler(httpd_req_t *req);

            // Designed to handle HTTP request
            esp_err_t StartOtaHandler(httpd_req_t *req);

            // Designed to handle HTTP request
            esp_err_t GetOtaProgressHandler(httpd_req_t *req);

            // Designed to handle HTTP request
            esp_err_t CheckFirmwareHandler(httpd_req_t *req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticRootHandler(httpd_req_t* req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticSetWiFiHandler(httpd_req_t *req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticCalibrateHandler(httpd_req_t *req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticAdjustPulsesHandler(httpd_req_t *req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticSetSleepModeHandler(httpd_req_t *req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticGetCurrentTimeHandler(httpd_req_t* req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticSetEmailHandler(httpd_req_t* req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticStartOtaHandler(httpd_req_t* req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticGetOtaProgressHandler(httpd_req_t* req);

            // Designed to handle HTTP request with wrap in static method
            static esp_err_t StaticCheckFirmwareHandler(httpd_req_t* req);

    };
};

