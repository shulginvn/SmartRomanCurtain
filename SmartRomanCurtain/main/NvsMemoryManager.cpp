
#include "NvsMemoryManager.h"

namespace SmartRomanCurtain
{
    // Designed to setup configure
    void NvsMemoryManager::Initialize()
    {
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);
    }

    // Designed to store data in flash memory
    void NvsMemoryManager::WriteDataToFlash(const char* key, int32_t value)
    {
        nvs_handle_t my_handle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            ESP_LOGI(TAG.c_str(), "Error opening NVS.");
        } else {
            err = nvs_set_i32(my_handle, key, value);
            if (err == ESP_OK) {
                ESP_LOGI(TAG.c_str(), "Data saved in NVS successfully.");
                err = nvs_commit(my_handle);
            } else {
                ESP_LOGI(TAG.c_str(), "The data was saved with an error.");
            }
            nvs_close(my_handle);
        }
    }

    // Designed to read data from flash memory
    void NvsMemoryManager::ReadDataFromFlash(const char* key, int32_t* value)
    {
        nvs_handle_t my_handle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            ESP_LOGI(TAG.c_str(), "Error opening NVS.");
        } else {
            err = nvs_get_i32(my_handle, key, value);
            switch (err) {
                case ESP_OK:
                    ESP_LOGI(TAG.c_str(), "NVS readed value: %d", *value);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    ESP_LOGI(TAG.c_str(), "Value not found in NVS.");
                    break;
                default:
                    ESP_LOGI(TAG.c_str(), "NVS reading error.");
                    break;
            }
            nvs_close(my_handle);
        }
    }

    // Designed to store data from flash memory
    void NvsMemoryManager::WriteDataToFlash(const char* key, const char* value)
    {
        nvs_handle_t my_handle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            ESP_LOGI(TAG.c_str(), "Error opening NVS.");
        } else {
            err = nvs_set_str(my_handle, key, value);
            if (err == ESP_OK) {
                ESP_LOGI(TAG.c_str(), "Data saved in NVS successfully.");
                err = nvs_commit(my_handle);
            } else {
                ESP_LOGI(TAG.c_str(), "The data was saved with an error.");
            }
            nvs_close(my_handle);
        }
    }

    // Designed to read data from flash memory
    int32_t NvsMemoryManager::ReadDataFromFlash(const char* key, const char* value)
    {
        nvs_handle_t my_handle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);

        if (err != ESP_OK) {
            ESP_LOGI(TAG.c_str(), "Error opening NVS.");
        } else {
            ESP_LOGI(TAG.c_str(), "nvs_get_str");
            size_t length = 128;
            err = nvs_get_str(my_handle, key, (char*)value, &length);
            ESP_LOGI(TAG.c_str(), "nvs_get_str end %d", length);
            switch (err) {
                case ESP_OK:
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    ESP_LOGI(TAG.c_str(), "Value not found in NVS.");
                    break;
                default:
                    ESP_LOGI(TAG.c_str(), "NVS reading error.");
                    break;
            }
            nvs_close(my_handle);
        }

        return err;
    }

}
