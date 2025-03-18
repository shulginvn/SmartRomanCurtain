
#include "WiFiConnector.h"


namespace SmartRomanCurtain {

    const char* WiFiConnector::IPV6_ADDR_TYPES[] = {
        "ESP_IP6_ADDR_IS_UNKNOWN",
        "ESP_IP6_ADDR_IS_GLOBAL",
        "ESP_IP6_ADDR_IS_LINK_LOCAL",
        "ESP_IP6_ADDR_IS_SITE_LOCAL",
        "ESP_IP6_ADDR_IS_UNIQUE_LOCAL",
        "ESP_IP6_ADDR_IS_IPV4_MAPPED_IPV6"
    };

    // Designed for wrap handle callback on got IPv6 address
    void WiFiConnector::OnGotIpV6Wrapper(void *arg, esp_event_base_t event_base,
            int32_t event_id, void *event_data)
    {
        WiFiConnector* self = static_cast<WiFiConnector*>(arg);
        if (self) {
            return self->OnGotIpV6(event_base, event_id, event_data);
        }
    }

    // Designed for handle callback on got IPv6 address
    void WiFiConnector::OnGotIpV6(esp_event_base_t event_base,
            int32_t event_id, void *event_data)
    {
        ip_event_got_ip6_t *event = (ip_event_got_ip6_t *)event_data;
        if (!IsOurNetif(TAG, event->esp_netif)) {
            ESP_LOGW(TAG, "Got IPv6 from another netif: ignored");
            return;
        }
        esp_ip6_addr_type_t ipv6_type = esp_netif_ip6_get_addr_type(&event->ip6_info.ip);
        ESP_LOGI(TAG, "Got IPv6 event: Interface \"%s\" address: " IPV6STR ", type: %s", esp_netif_get_desc(event->esp_netif),
                 IPV62STR(event->ip6_info.ip), IPV6_ADDR_TYPES[ipv6_type]);
        if (ipv6_type == ESP_IP6_ADDR_IS_LINK_LOCAL) {
            memcpy(&_ipv6Addr, &event->ip6_info.ip, sizeof(_ipv6Addr));
        }

        // ###
        esp_netif_t *netif = NULL;
        esp_ip6_addr_t ip6[MAX_IP6_ADDRS_PER_NETIF];
        int ip6_addrs = esp_netif_get_all_ip6(netif, ip6);
        for (int j = 0; j < ip6_addrs; ++j) {
            esp_ip6_addr_type_t ipv6_type = esp_netif_ip6_get_addr_type(&(ip6[j]));
            ESP_LOGI(TAG, "- IPv6 address: " IPV6STR ", type: %s", IPV62STR(ip6[j]), IPV6_ADDR_TYPES[ipv6_type]);
        }
    }

    // Designed for wrap handle callback on got IP address
    void WiFiConnector::OnGotIpWrapper(void *arg, esp_event_base_t event_base,
            int32_t event_id, void *event_data)
    {
        WiFiConnector* self = static_cast<WiFiConnector*>(arg);
        if (self) {
            return self->OnGotIp(event_base, event_id, event_data);
        }
    }

    // Designed for handle callback on got IP address
    void WiFiConnector::OnGotIp(esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        if (!IsOurNetif(TAG, event->esp_netif)) {
            ESP_LOGW(TAG, "Got IPv4 from another interface \"%s\": ignored", esp_netif_get_desc(event->esp_netif));
            return;
        }
        ESP_LOGI(TAG, "Got IPv4 event: Interface \"%s\" address: " IPSTR, esp_netif_get_desc(event->esp_netif), IP2STR(&event->ip_info.ip));
        memcpy(&_ipAddr, &event->ip_info.ip, sizeof(_ipAddr));

        // ###
        esp_netif_t *netif = NULL;
        esp_netif_ip_info_t ip;
        for (int i = 0; i < esp_netif_get_nr_of_ifs(); ++i) {
            netif = esp_netif_next(netif);
            if (IsOurNetif(TAG, netif)) {
                ESP_LOGI(TAG, "Connected to %s", esp_netif_get_desc(netif));
                ESP_ERROR_CHECK(esp_netif_get_ip_info(netif, &ip));

                ESP_LOGI(TAG, "- IPv4 address: " IPSTR, IP2STR(&ip.ip));
            }
        }
        // ###

        if (_isApModeActive) {
            ESP_LOGI(TAG, "Successfully connected to STA. Disabling AP...");

            // Deactivate AP
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

            // Reset AP active flag an counter
            _isApModeActive = false;
        }
        _retryCount = 0;
    }

    // Designed for wrap handle callback on WI-FI connect
    void WiFiConnector::OnWiFiConnectWrapper(void *arg, esp_event_base_t event_base,
            int32_t event_id, void *event_data)
    {
        WiFiConnector* self = static_cast<WiFiConnector*>(arg);
        if (self) {
            return self->OnWiFiConnect(self->GetNetif(), event_base, event_id, event_data);
        }
    }

    // Designed for get netif instance
    esp_netif_t* WiFiConnector::GetNetif()
    {
        return _espNetif;
    }

    // Designed for handle callback on WI-FI connect
    void WiFiConnector::OnWiFiConnect(esp_netif_t* esp_netif, esp_event_base_t event_base,
            int32_t event_id, void *event_data)
    {
        esp_netif_create_ip6_linklocal(esp_netif);
    }

    // Designed for wrap handle callback on WI-FI disconnect
    void WiFiConnector::OnWiFiDisconnectWrapper(void *arg, esp_event_base_t event_base,
            int32_t event_id, void *event_data)
    {
        WiFiConnector* self = static_cast<WiFiConnector*>(arg);
        if (self) {
            return self->OnWiFiDisconnect(event_base, event_id, event_data);
        }
    }

    // Designed for handle callback on WI-FI disconnect
    void WiFiConnector::OnWiFiDisconnect(esp_event_base_t event_base,
            int32_t event_id, void *event_data)
    {
        ESP_LOGI(TAG, "OnWiFiDisconnect with ssid=%s, pwd=%s", _ssid, _password);

        if (_retryCount < MAX_RETRY_COUNT) {
            ESP_LOGI(TAG, "Wi-Fi disconnected, trying to reconnect... Attempt %d/%d", _retryCount + 1, MAX_RETRY_COUNT);
            esp_err_t err = esp_wifi_connect();
            if (err == ESP_ERR_WIFI_NOT_STARTED) {
                return;
            }
            ESP_ERROR_CHECK(err);
            _retryCount++;
        } else if (!_isApModeActive) {

            ESP_LOGI(TAG, "Max retry count reached. Switching to AP mode...");

            wifi_config_t wifiApConfig;

            strncpy((char *)wifiApConfig.ap.ssid, "RomanCurtain", sizeof(wifiApConfig.ap.ssid) - 1);
            wifiApConfig.ap.ssid[sizeof(wifiApConfig.ap.ssid) - 1] = '\0';
            wifiApConfig.ap.ssid_len = strlen("RomanCurtain");
            strncpy((char *)wifiApConfig.ap.password, "12345678", sizeof(wifiApConfig.ap.password) - 1);
            wifiApConfig.ap.password[sizeof(wifiApConfig.ap.password) - 1] = '\0';
            wifiApConfig.ap.channel = 1;
            wifiApConfig.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
            wifiApConfig.ap.ssid_hidden = 0;
            wifiApConfig.ap.max_connection = 4;
            wifiApConfig.ap.beacon_interval = 100;
            wifiApConfig.ap.pairwise_cipher = WIFI_CIPHER_TYPE_NONE;
            wifiApConfig.ap.ftm_responder = false;

            // Set part of MAC in SSID
            uint8_t macSoftAp[6];
            esp_err_t ret = esp_read_mac(macSoftAp, ESP_MAC_WIFI_SOFTAP);
            if (ret != ESP_OK) {
                ESP_LOGE("MAC", "Failed to get MAC address: %s", esp_err_to_name(ret));
            }

            char macSuffix[9] = {0};
            snprintf(macSuffix, sizeof(macSuffix), "%02X%02X%02X%02X", macSoftAp[2], macSoftAp[3], macSoftAp[4], macSoftAp[5]);

            char finalSsid[50] = {0};
            snprintf(finalSsid, sizeof(finalSsid), "%s_%s", wifiApConfig.ap.ssid, macSuffix);

            strncpy((char *)wifiApConfig.ap.ssid, finalSsid, strlen(finalSsid));
            wifiApConfig.ap.ssid_len = strlen(finalSsid);

            // Switch to AP mode
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
            ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifiApConfig));
            ESP_ERROR_CHECK(esp_wifi_start());

            // Reset counter
            _retryCount = 0;

            // Set AP active flag
            _isApModeActive = true;

            // Try connect to STA
            esp_err_t err = esp_wifi_connect();
            if (err == ESP_ERR_WIFI_NOT_STARTED) {
                return;
            }
            ESP_ERROR_CHECK(err);
        } else {
            // If in mode APSTA trying to connect to STA
            ESP_LOGI(TAG, "Retrying STA connection in APSTA mode...");
            esp_err_t err = esp_wifi_connect();
            if (err == ESP_ERR_WIFI_NOT_STARTED) {
                return;
            }
            ESP_ERROR_CHECK(err);
        }
    }

    // Designed for initiate WI-FI connection
    void WiFiConnector::Connect(const char* ssid, const char* password)
    {
        strcpy(_ssid, ssid);
        strcpy(_password, password);

        Start();

        // iterate over active interfaces, and print out IPs of "our" netifs
        esp_netif_t *netif = NULL;
        esp_netif_ip_info_t ip;
        for (int i = 0; i < esp_netif_get_nr_of_ifs(); ++i) {
            netif = esp_netif_next(netif);
            if (IsOurNetif(TAG, netif)) {
                ESP_LOGI(TAG, "Connected to %s", esp_netif_get_desc(netif));
                ESP_ERROR_CHECK(esp_netif_get_ip_info(netif, &ip));

                ESP_LOGI(TAG, "- IPv4 address: " IPSTR, IP2STR(&ip.ip));
            }
        }
    }

    // Designed for initiate WI-FI disconnect
    void WiFiConnector::Disconnect()
    {
        Stop();
        // ESP_ERROR_CHECK(esp_unregister_shutdown_handler(&stop));
    }

    // Designed for WI-FI start
    void WiFiConnector::Start()
    {
        char *desc;
        esp_netif_create_default_wifi_ap();

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); // @suppress("Invalid arguments")
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));

        esp_netif_inherent_config_t esp_netif_config = ESP_NETIF_INHERENT_DEFAULT_WIFI_STA();
        // Prefix the interface description with the module TAG
        // Warning: the interface desc is used in tests to capture actual connection details (IP, gw, mask)
        asprintf(&desc, "%s: %s", TAG, esp_netif_config.if_desc);
        esp_netif_config.if_desc = desc;
        esp_netif_config.route_prio = 128;
        _espNetif = esp_netif_create_wifi(WIFI_IF_STA, &esp_netif_config);
        free(desc);
        esp_wifi_set_default_wifi_sta_handlers();

        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &OnWiFiDisconnectWrapper, this));
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &OnGotIpWrapper, this));
    #ifdef CONFIG_EXAMPLE_CONNECT_IPV6
        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &OnWiFiConnectWrapper, this));
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_GOT_IP6, &OnGotIpV6Wrapper, this));
    #endif

        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

        wifi_config_t wifiStaConfig = {};
        wifiStaConfig.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
        wifiStaConfig.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
        wifiStaConfig.sta.threshold.rssi = WIFI_SCAN_RSSI_THRESHOLD;
        wifiStaConfig.sta.threshold.authmode = WIFI_AUTH_OPEN;

        strncpy((char *)wifiStaConfig.sta.ssid, _ssid, strlen(_ssid));
        strncpy((char *)wifiStaConfig.sta.password, _password, strlen(_password));

        // Set mode and start connect
        ESP_LOGI(TAG, "Connecting to %s...", wifiStaConfig.sta.ssid);
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiStaConfig));
        ESP_ERROR_CHECK(esp_wifi_start());

        // Set DNS
        mdns_init();
        mdns_hostname_set("roman-curtain");

        if (strlen(_ssid) > 0 && strlen(_password) > 0) {
            esp_wifi_connect();
        }
    }

    // Designed for WI-FI stop
    void WiFiConnector::Stop()
    {
        ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &OnWiFiDisconnectWrapper));
        ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &OnGotIpWrapper));
    #ifdef CONFIG_EXAMPLE_CONNECT_IPV6
        ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_GOT_IP6, &OnGotIpV6Wrapper));
        ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &OnWiFiConnectWrapper));
    #endif
        esp_err_t err = esp_wifi_stop();
        if (err == ESP_ERR_WIFI_NOT_INIT) {
            return;
        }
        ESP_ERROR_CHECK(err);
        ESP_ERROR_CHECK(esp_wifi_deinit());
        ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(_espNetif));
        esp_netif_destroy(_espNetif);
        _espNetif = nullptr;
    }

    // Designed for check netif
    bool WiFiConnector::IsOurNetif(const char *prefix, esp_netif_t *netif)
    {
        return strncmp(prefix, esp_netif_get_desc(netif), strlen(prefix) - 1) == 0;
    }

} /* namespace SmartRomanCurtain */
