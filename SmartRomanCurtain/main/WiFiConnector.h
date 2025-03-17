#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sdkconfig.h"

#include "esp_err.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_log.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include <mdns.h>

#include "lwip/err.h"
#include "lwip/sys.h"

#include "OtaUpdater.h"

namespace SmartRomanCurtain {

    class WiFiConnector {
        public:
            // Designed for initiate WI-FI connection
            void Connect(const char* ssid, const char* password);

            // Designed for initiate WI-FI disconnect
            void Disconnect();

            // Designed to pass OtaUpdater
            void Set(OtaUpdater* otaUpdater);

        private:

            const char *TAG = "example_connect";

            /* types of ipv6 addresses to be displayed on ipv6 events */
            static const char* IPV6_ADDR_TYPES[];
            const int32_t MAX_IP6_ADDRS_PER_NETIF = 5;
            const int32_t MAX_RETRY_COUNT = 10;
            const int32_t WIFI_SCAN_RSSI_THRESHOLD = -127;

            char _ssid[32] = {0};
            char _password[64] = {0};

            int32_t _retryCount = 0;
            bool _isApModeActive = false;

            esp_ip4_addr_t _ipAddr = {};
            esp_ip6_addr_t _ipv6Addr = {};
            esp_netif_t* _espNetif = nullptr;
            OtaUpdater* _otaUpdater = nullptr;

            // Designed for WI-FI start
            void Start();

            // Designed for WI-FI stop
            void Stop();

            // Designed for wrap handle callback on got IP address
            static void OnGotIpWrapper(void *arg, esp_event_base_t event_base,
                    int32_t event_id, void *event_data);

            // Designed for handle callback on got IP address
            void OnGotIp(esp_event_base_t event_base,
                    int32_t event_id, void *event_data);

            // Designed for wrap handle callback on got IPv6 address
            static void OnGotIpV6Wrapper(void *arg, esp_event_base_t event_base,
                    int32_t event_id, void *event_data);

            // Designed for handle callback on got IPv6 address
            void OnGotIpV6(esp_event_base_t event_base,
                    int32_t event_id, void *event_data);

            // Designed for check netif
            bool IsOurNetif(const char *prefix, esp_netif_t *netif);

            // Designed for wrap handle callback on WI-FI connect
            static void OnWiFiConnectWrapper(void *arg, esp_event_base_t event_base,
                    int32_t event_id, void *event_data);

            // Designed for handle callback on WI-FI connect
            void OnWiFiConnect(esp_netif_t* esp_netif, esp_event_base_t event_base,
                    int32_t event_id, void *event_data);

            // Designed for wrap handle callback on WI-FI disconnect
            static void OnWiFiDisconnectWrapper(void *arg, esp_event_base_t event_base,
                    int32_t event_id, void *event_data);

            // Designed for handle callback on WI-FI disconnect
            void OnWiFiDisconnect(esp_event_base_t event_base,
                    int32_t event_id, void *event_data);

            // Designed for get netif instance
            esp_netif_t* GetNetif();

    };
} /* namespace SmartRomanCurtain */

