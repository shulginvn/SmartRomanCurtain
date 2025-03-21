
#include "WebServerManager.h"

namespace SmartRomanCurtain
{
    extern const uint8_t rootCA_crt_start[] asm("_binary_rootCA_crt_start"); // @suppress("Unused variable declaration in file scope")
    extern const uint8_t rootCA_crt_end[] asm("_binary_rootCA_crt_end"); // @suppress("Unused variable declaration in file scope")

    const char *WebServerManager::_htmlFormServiceMenu =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta charset=\"UTF-8\">"
    "<title>Home Curtain Configuration Panel</title>"

    "<style>"
    "body { font-family: Arial, sans-serif; margin: 20px; }"
    "h1 { color: #333; }"
    "table { width: 100%; border-collapse: collapse; table-layout: fixed; }"
    "td { padding: 8px; text-align: left; vertical-align: middle; }"
    "form { margin-bottom: 20px; }"
    "input[type='text'], input[type='number'], input[type='password'], input[type='checkbox'], select {"
    " padding: 8px; margin: 5px 0; width: 100%; box-sizing: border-box;"
    " border: 1px solid #ccc; border-radius: 4px;"
    "}"
    "input[type='checkbox'] {"
    "width: auto;"
    "margin: 5px 0;"
    "}"
    "button {"
    " background-color: #4CAF50; color: white; padding: 10px 15px; border: none;"
    " border-radius: 4px; cursor: pointer; font-size: 16px;"
    "}"
    "button:hover { background-color: #45a049; }"
    "</style>"

    "</head>"

    "<body>"

    "<h2>Настройка уникального идентификатора</h2>"
    "<div>"
    " <label>Номер устройства:</label>"
    " <input type='text' id='uniqueId' placeholder='Enter your unique Id' required><br>"
    "<button onclick=\"setUniqueId()\">Отправить</button>"
    "</div>"

    "<script>"

    "function setUniqueId() {"
    "    const uniqueId = document.getElementById('uniqueId').value;"
    "    fetch('/setUniqueId', {"
    "        method: 'POST',"
    "        headers: { 'Content-Type': 'application/json' },"
    "        body: JSON.stringify({ id: uniqueId })"
    "    })"
    "    .then(response => response.json())"
    "    .then(data => {"
    "       if (data.status === \"success\") {"
    "           alert(\"Unique Id успешно принят.\");"
    "       } else {"
    "           alert(\"Ошибка: \" + data.message);"
    "       }"
    "})"
    "    .catch(error => console.error('Error:', error));"
    "}"

    "</script>"

    "</body>"
    "</html>";

const char *WebServerManager::_htmlForm =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta charset=\"UTF-8\">"
    "<title>Home Curtain Configuration Panel</title>"

    "<style>"
    "body { font-family: Arial, sans-serif; margin: 20px; }"
    "h1 { color: #333; }"
    "table { width: 100%; border-collapse: collapse; table-layout: fixed; }"
    "td { padding: 8px; text-align: left; vertical-align: middle; }"
    "form { margin-bottom: 20px; }"
    "input[type='text'], input[type='number'], input[type='password'], input[type='checkbox'], select {"
    " padding: 8px; margin: 5px 0; width: 100%; box-sizing: border-box;"
    " border: 1px solid #ccc; border-radius: 4px;"
    "}"
    "input[type='checkbox'] {"
    "width: auto;"
    "margin: 5px 0;"
    "}"
    "button {"
    " background-color: #4CAF50; color: white; padding: 10px 15px; border: none;"
    " border-radius: 4px; cursor: pointer; font-size: 16px;"
    "}"
    "button:hover { background-color: #45a049; }"
    "#statusMessage { margin-top: 15px; font-size: 16px; color: #333; }"
    "progress { width: 100%; height: 25px; border-radius: 12px; overflow: hidden; background-color: #eee; border: 1px solid #ccc; margin: 5px 0 }"
    "progress::-webkit-progress-bar { background-color: #eee; border-radius: 12px; }"
    "progress::-webkit-progress-value { background-color: #4CAF50; border-radius: 12px; }"
    "progress::-moz-progress-bar { background-color: #4CAF50; border-radius: 12px; }"
    "</style>"

    "</head>"
    "<body>"
    /*"<h1>Конфигурационная панель Римской Шторы</h1>"*/

    "<h2>Настройка WI-FI</h2>"
    "<form action=\"/submit\" method=\"post\">"
    " <label for=\"login\">Логин:</label>"
    " <input type=\"text\" id=\"login\" name=\"login\" placeholder=\"Enter your login\" required><br>"
    " <label for=\"password\">Пароль:</label>"
    " <input type=\"password\" id=\"password\" name=\"password\" placeholder=\"Enter your password\" required><br>"
    " <button type=\"submit\">Настроить</button>"
    "</form>"

    "<h2>Связывание по email</h2>"
    "<div>"
    " <label for='email'>Значение Email:</label>"
    " <input type='text' id='email' placeholder='Enter your email' required><br>"
    "<button onclick=\"setEmail()\">Отправить</button>"
    "</div>"

    "<h2>Настройка режима сна</h2>"
    "<form id=\"sleepForm\" action=\"/setSleepMode\" method=\"post\">"
    "    <table>"
    "        <tr>"
    "            <td><b>Интервал 1.</b></td>"
    "            <td><input type=\"checkbox\" id=\"enableSleepInterval1\" name=\"enableSleepInterval1\"></td>"
    "            <td>Начало сна (часы, 0-23):</td>"
    "            <td><input type=\"number\" id=\"sleepStartHourInterval1\" name=\"sleepStartHourInterval1\" min=\"0\" max=\"23\" required></td>"
    "            <td>Начало сна (минуты, 0-59):</td>"
    "            <td><input type=\"number\" id=\"sleepStartMinuteInterval1\" name=\"sleepStartMinuteInterval1\" min=\"0\" max=\"59\" required></td>"
    "            <td>Длительность сна (часы):</td>"
    "            <td><input type=\"number\" id=\"sleepDurationInterval1\" name=\"sleepDurationInterval1\" min=\"1\" required></td>"
    "        </tr>"
    "        <tr>"
    "            <td><b>Интервал 2.</b></td>"
    "            <td><input type=\"checkbox\" id=\"enableSleepInterval2\" name=\"enableSleepInterval2\"></td>"
    "            <td>Начало сна (часы, 0-23):</td>"
    "            <td><input type=\"number\" id=\"sleepStartHourInterval2\" name=\"sleepStartHourInterval2\" min=\"0\" max=\"23\" required></td>"
    "            <td>Начало сна (минуты, 0-59):</td>"
    "            <td><input type=\"number\" id=\"sleepStartMinuteInterval2\" name=\"sleepStartMinuteInterval2\" min=\"0\" max=\"59\" required></td>"
    "            <td>Длительность (часы):</td>"
    "            <td><input type=\"number\" id=\"sleepDurationInterval2\" name=\"sleepDurationInterval2\" min=\"1\" required></td>"
    "        </tr>"
    "    </table>"
    "    <button type=\"button\" onclick=\"configureSleepMode()\">Сохранить</button>"
    "</form>"
    "<div>"
    " <label for='currentTime'>Текущее время:</label>"
    " <input type='text' id='currentTime' placeholder='Часы и минуты' readonly><br>"
    "<button type=\"button\" onclick=\"getCurrentTime()\">Прочитать</button>"
    "</div>"

    "<h2>Настройка калибровочных параметров</h2>"
    "<div>"
    " <label for='calibration-up'>Калибровочное значение:</label>"
    " <input type='text' id='calibration' placeholder='Введите значение'><br>"
    " <button onclick='calibrate(\"up\", \"read\")'>Прочитать</button>"
    " <button onclick='calibrate(\"up\", \"write\")'>Записать</button>"
    " <button onclick='calibrate(\"up\", \"save\")'>Сохранить</button>"
    "</div>"
    "</br>"
    "<div>"
    " <label for='pulses-up'>Текущее количество импульсов:</label>"
    " <input type='text' id='pulses' placeholder='Введите значение'><br>"
    " <button onclick='adjustPulses(\"up\", \"read\")'>Прочитать</button>"
    " <button onclick='adjustPulses(\"up\", \"write\")'>Записать</button>"
    "</div>"
    "</br>"

    "<h2>Обновление системы</h2>"
    "<div>"
    "<label id=\"statusMessage\">Ожидание действий.</label>"
    "<progress id=\"progressBar\" value=\"0\" max=\"100\"></progress>"
    "<button id=\"checkFirmwareButton\" style=\"margin-right: 5px;\">Проверить</button>"
    "<button id=\"updateFirmwareButton\">Запустить</button>"
    "</div>"

    // Scripts

    "<script>"

    "function setEmail() {"
    "    const email = document.getElementById('email').value;"
    "    fetch('/setEmail', {"
    "        method: 'POST',"
    "        headers: { 'Content-Type': 'application/json' },"
    "        body: JSON.stringify({ email: email })"
    "    })"
    "    .then(response => response.text())"
    "    .then(data => alert(data))"
    "    .catch(error => console.error('Error:', error));"
    "}"

    "function calibrate(direction, action) {"
    "  let value = '';"
    "  if (action === 'write') {"
    "    value = document.getElementById('calibration').value;"
    "  }"
    "  fetch('/calibrate', {"
    "    method: 'POST',"
    "    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },"
    "    body: `action=${action}&direction=${direction}&value=${encodeURIComponent(value)}`"
    "  })"
    "  .then(response => response.text())"
    "  .then(data => {"
    "    if (action === 'read') {"
    "      document.getElementById('calibration').value = data;"
    "    }"
    "    alert(data);"
    "  });"
    "}"

    "function adjustPulses(direction, action) {"
    "  let value = '';"
    "  if (action === 'write') {"
    "    value = document.getElementById('pulses').value;"
    "  }"
    "  fetch('/adjustPulses', {"
    "    method: 'POST',"
    "    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },"
    "    body: `action=${action}&direction=${direction}&value=${encodeURIComponent(value)}`"
    "  })"
    "  .then(response => response.text())"
    "  .then(data => {"
    "    if (action === 'read') {"
    "      document.getElementById('pulses').value = data;"
    "    }"
    "    alert(data);"
    "  });"
    "}"

    "function getCurrentTime() {"
    "    fetch('/getCurrentTime', {"
    "        method: 'POST',"
    "        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },"
    "        body: ``"
    "    }).then(response => response.text()).then(data => { "
    "document.getElementById('currentTime').value = data;"
    "alert(data);"
    "});"
    "}"

    "function configureSleepMode() {"
    "    const enableSleepInterval1 = document.getElementById('enableSleepInterval1').checked ? 1 : 0;"
    "    const sleepStartHourInterval1 = document.getElementById('sleepStartHourInterval1').value;"
    "    const sleepStartMinuteInterval1 = document.getElementById('sleepStartMinuteInterval1').value;"
    "    const sleepDurationInterval1 = document.getElementById('sleepDurationInterval1').value;"
    "    const enableSleepInterval2 = document.getElementById('enableSleepInterval2').checked ? 1 : 0;"
    "    const sleepStartHourInterval2 = document.getElementById('sleepStartHourInterval2').value;"
    "    const sleepStartMinuteInterval2 = document.getElementById('sleepStartMinuteInterval2').value;"
    "    const sleepDurationInterval2 = document.getElementById('sleepDurationInterval2').value;"
    "    fetch('/configureSleepMode', {"
    "        method: 'POST',"
    "        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },"
    "        body: `enableSleepInterval1=${enableSleepInterval1}&sleepStartHourInterval1=${sleepStartHourInterval1}&sleepStartMinuteInterval1=${sleepStartMinuteInterval1}&sleepDurationInterval1=${sleepDurationInterval1}&enableSleepInterval2=${enableSleepInterval2}&sleepStartHourInterval2=${sleepStartHourInterval2}&sleepStartMinuteInterval2=${sleepStartMinuteInterval2}&sleepDurationInterval2=${sleepDurationInterval2}`"
    "    }).then(response => response.text()).then(data => alert(data));"
    "}"

    "document.getElementById('updateFirmwareButton').addEventListener('click', function() {"
    "    fetch('/startOTA')"
    "        .then(response => {"
    "            if (!response.ok) {"
    "                throw new Error('Ошибка сети');"
    "            }"
    "            return response.text();"
    "        })"
    "        .then(data => {"
    "            document.getElementById('statusMessage').textContent = data;"
    "            checkOtaProgress();"
    "        })"
    "        .catch(error => {"
    "            console.error('Ошибка:', error);"
    "            document.getElementById('statusMessage').textContent = 'Ошибка при запросе';"
    "        });"
    "});"

    "function checkOtaProgress() {"
    "    fetch('/otaProgress')"
    "        .then(response => response.json())"
    "        .then(data => {"
    "            document.getElementById('progressBar').value = data.progress;"
    "            document.getElementById('statusMessage').textContent = data.statusMessage;"
    "            if (data.progress < 100 && data.statusMessage !== 'Прошивка успешно обновлена') {"
    "                setTimeout(checkOtaProgress, 100);"
    "            }"
    "        })"
    "        .catch(error => {"
    "            console.error('Ошибка:', error);"
    "        });"
    "}"

    "document.getElementById('checkFirmwareButton').addEventListener('click', function() {"
    "    fetch('/checkFirmware')"
    "        .then(response => response.json())"
    "        .then(data => {"
    "            document.getElementById('statusMessage').textContent = data.statusMessage;"
    "        })"
    "        .catch(error => {"
    "            console.error('Ошибка:', error);"
    "        });"
    "});"

    "</script>"

    "</body>"
    "</html>";

    // Designed to set instance
    void WebServerManager::Set(NvsMemoryManager* nvsMemoryManager)
    {
        _nvsMemoryManager = nvsMemoryManager;
    }

    // Designed to set instance
    void WebServerManager::Set(MotorController* motorController)
    {
        _motorController = motorController;
    }

    // Designed to set instance
    void WebServerManager::Set(DeepSleepNtp* deepSleepNtp)
    {
        _deepSleepNtp = deepSleepNtp;
    }

    // Designed to set instance
    void WebServerManager::Set(OtaUpdater* otaUpdater)
    {
        _otaUpdater = otaUpdater;
    }

    // Designed for start MQTT with actual login and password
    void WebServerManager::Set(const std::function<void (const std::string&, const std::string&)> setMqttAuthInfoWithInitCallback)
    {
        _setMqttAuthInfoWithInitCallback = setMqttAuthInfoWithInitCallback;
    }

    // Designed to get login
    const char* WebServerManager::GetLogin()
    {
        return _login;
    }

    // Designed to get password
    const char* WebServerManager::GetPassword()
    {
        return _password;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::RootHandler(httpd_req_t *req)
    {
        httpd_resp_set_type(req, "text/html");

        char query[100];
        if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {

            ESP_LOGI(TAG.c_str(), "BUF: %s", query);
            // Check "page" parameter on exist
            char page[20];
            if (httpd_query_key_value(query, "page", page, sizeof(page)) == ESP_OK) {
                if (strcmp(page, "serviceMenu") == 0) {
                    // Return service menu
                    httpd_resp_send(req, _htmlFormServiceMenu, HTTPD_RESP_USE_STRLEN);
                    return ESP_OK;
                }
            }
        }

        httpd_resp_send(req, _htmlForm, HTTPD_RESP_USE_STRLEN);

        return ESP_OK;

    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::SetWiFiHandler(httpd_req_t *req)
    {
        char buf[100] = {0};

        int ret, remaining = req->content_len;

        while (remaining > 0) {

            ret = httpd_req_recv(req, buf, std::min(static_cast<size_t>(remaining), sizeof(buf)));


            ESP_LOGI(TAG.c_str(), "BUF: %s", buf);

            if (ret <= 0) {
                return ESP_FAIL;

            }

            remaining -= ret;

            char *login_ptr = strstr(buf, "login=");

            char *password_ptr = strstr(buf, "password=");

            if (login_ptr) {
                sscanf(login_ptr, "login=%49[^&]", _login);
            }

            if (password_ptr) {
                sscanf(password_ptr, "password=%49s", _password);
            }
        }

        ESP_LOGI(TAG.c_str(), "Login: %s, Password: %s", _login, _password);

        httpd_resp_set_type(req, "text/html");

        httpd_resp_sendstr(req, "<h1>Login and Password was received. The curtain will be rebooted.</h1><a href=\"/\">Back</a>");

        _nvsMemoryManager->WriteDataToFlash("login", _login);
        _nvsMemoryManager->WriteDataToFlash("password", _password);

        esp_restart();

        return ESP_OK;
    }


    // Designed to handle HTTP request
    esp_err_t WebServerManager::CalibrateHandler(httpd_req_t *req)
    {
        char buf[128] = {0};
        int ret = httpd_req_recv(req, buf, std::min(req->content_len, sizeof(buf) - 1));
        if (ret <= 0) {
            ESP_LOGE(TAG.c_str(), "Failed to receive data");
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive data");
            return ESP_FAIL;
        }

        buf[ret] = '\0';

        char action[10] = {0}, direction[10] = {0};
        char value_str[10] = {0};
        int value = 0;

        if (httpd_query_key_value(buf, "action", action, sizeof(action)) != ESP_OK) {
            ESP_LOGE(TAG.c_str(), "Missing or invalid 'action'");
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid or missing 'action' parameter");
            return ESP_FAIL;
        }

        if (httpd_query_key_value(buf, "direction", direction, sizeof(direction)) != ESP_OK) {
            ESP_LOGE(TAG.c_str(), "Missing or invalid 'direction'");
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid or missing 'direction' parameter");
            return ESP_FAIL;
        }

        if (httpd_query_key_value(buf, "value", value_str, sizeof(value_str)) == ESP_OK) {
            value = atoi(value_str);
        }

        ESP_LOGI(TAG.c_str(), "Received action: %s, direction: %s, value: %d", action, direction, value);

        if (strcmp(action, "read") == 0) {
            int calibrationValue = _motorController->GetBaseCalibration();
            char response[32];
            snprintf(response, sizeof(response), "%d", calibrationValue);
            httpd_resp_set_type(req, "text/plain");
            httpd_resp_send(req, response, strlen(response));
        } else if (strcmp(action, "write") == 0) {
            if (strcmp(direction, "up") == 0) {
                _motorController->SetBaseCalibration(value);
            }
            httpd_resp_sendstr(req, "Calibration value updated successfully.");
        } else if (strcmp(action, "save") == 0) {
            if (strcmp(direction, "up") == 0) {
                _nvsMemoryManager->WriteDataToFlash("bc", _motorController->GetBaseCalibration());
            }
            httpd_resp_sendstr(req, "Calibration value saved successfully.");
        } else {
            ESP_LOGE(TAG.c_str(), "Invalid action: %s", action);
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid action");
            return ESP_FAIL;
        }

        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::AdjustPulsesHandler(httpd_req_t *req)
    {
        char buf[100] = {0};
        int ret = httpd_req_recv(req, buf, std::min(req->content_len, sizeof(buf) - 1));

        if (ret <= 0) {
            ESP_LOGE(TAG.c_str(), "Failed to receive data");
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive data");
            return ESP_FAIL;
        }

        buf[ret] = '\0';
        ESP_LOGI(TAG.c_str(), "Received data: %s", buf);

        char action[16] = {0};
        char direction[16] = {0};
        int value = 0;

        if (sscanf(buf, "action=%15[^&]&direction=%15[^&]&value=%d", action, direction, &value) < 2) {
            ESP_LOGE(TAG.c_str(), "Invalid parameters");
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid parameters");
            return ESP_FAIL;
        }

        if (strcmp(action, "read") == 0) {
            if (strcmp(direction, "up") == 0) {
                char response[16];
                snprintf(response, sizeof(response), "%d", _motorController->GetPositionInPulses());
                ESP_LOGI(TAG.c_str(), "Read pulses up: %d", _motorController->GetPositionInPulses());
                httpd_resp_sendstr(req, response);
            } else {
                ESP_LOGE(TAG.c_str(), "Invalid direction");
                httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid direction");
                return ESP_FAIL;
            }
        } else if (strcmp(action, "write") == 0) {
            if (strcmp(direction, "up") == 0) {
                _motorController->SetPositionInPulses(value);
                ESP_LOGI(TAG.c_str(), "Updated pulses up: %d", _motorController->GetPositionInPulses());
                httpd_resp_sendstr(req, "Pulses up updated successfully.");
            } else {
                ESP_LOGE(TAG.c_str(), "Invalid direction");
                httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid direction");
                return ESP_FAIL;
            }
        } else {
            ESP_LOGE(TAG.c_str(), "Invalid action");
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid action");
            return ESP_FAIL;
        }

        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::SetSleepModeHandler(httpd_req_t *req)
    {
        char content[400] = {0};
        int ret = httpd_req_recv(req, content, sizeof(content) - 1);
        if (ret <= 0) {
            ESP_LOGE(TAG.c_str(), "Failed to receive data");
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive data");
            return ESP_FAIL;
        }
        content[ret] = '\0';

        std::vector<DeepSleepNtp::StSleepModeParamas> sleepModeParams = {};
        sleepModeParams.resize(2);

        for (int32_t i = 0; i < sleepModeParams.size(); i++) {
            char *param;
            param = strstr(content, ("enableSleepInterval" + std::to_string(i+1) + "=").c_str());
            if (param) {
                sleepModeParams.at(i).enableSleep = atoi(param + strlen(("enableSleepInterval" + std::to_string(i+1) + "=").c_str())) > 0;
            }

            param = strstr(content, ("sleepStartHourInterval" + std::to_string(i+1) + "=").c_str());
            if (param) {
                sleepModeParams.at(i).sleepStartHour = atoi(param + strlen(("sleepStartHourInterval" + std::to_string(i+1) + "=").c_str()));
            }

            param = strstr(content, ("sleepStartMinuteInterval" + std::to_string(i+1) + "=").c_str());
            if (param) {
                sleepModeParams.at(i).sleepStartMinute = atoi(param + strlen(("sleepStartMinuteInterval" + std::to_string(i+1) + "=").c_str()));
            }

            param = strstr(content, ("sleepDurationInterval" + std::to_string(i+1) + "=").c_str());
            if (param) {
                sleepModeParams.at(i).sleepDurationHours = atoi(param + strlen(("sleepDurationInterval" + std::to_string(i+1) + "=").c_str()));
            }

            ESP_LOGI("WebServer", "Interval %d Sleep Mode: %s", i+1, sleepModeParams.at(i).enableSleep ? "ON" : "OFF");
            ESP_LOGI("WebServer", "Interval %d Sleep Start Hour: %d", i+1, sleepModeParams.at(i).sleepStartHour);
            ESP_LOGI("WebServer", "Interval %d Sleep Start Minute: %d", i+1, sleepModeParams.at(i).sleepStartMinute);
            ESP_LOGI("WebServer", "Interval %d Sleep Duration Hour: %d", i+1, sleepModeParams.at(i).sleepDurationHours);
        }

        _deepSleepNtp->SetSleepModeParams(sleepModeParams);

        httpd_resp_set_type(req, "text/plain");
        httpd_resp_send(req, "Настройки режима сна сохранены", HTTPD_RESP_USE_STRLEN);

        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::GetCurrentTimeHandler(httpd_req_t *req)
    {
        char response[16];
        snprintf(response, sizeof(response), "%02d:%02d", _deepSleepNtp->GetCurrentHour(), _deepSleepNtp->GetCurrentMinute());
        httpd_resp_sendstr(req, response);

        return ESP_OK;
    }

    // Designed to handle HTTP event callback
    esp_err_t WebServerManager::ProcessMqttAuthInfo(esp_http_client_event_t *evt)
    {
        switch ((int)evt->event_id) {
            case HTTP_EVENT_ON_DATA: {
                WebServerManager* self = static_cast<WebServerManager*>(evt->user_data);
                if (self) {
                    return self->SetMqttAuthInfo((char *)evt->data, evt->data_len);
                }
            }    break;
            default:
                break;
        }
        return ESP_OK;
    }

    // Designed to set JSON MQTT authenticate information
    esp_err_t WebServerManager::SetMqttAuthInfo(const char* data, const uint32_t length)
    {
        _mqttAuthData.append(data, length);
        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::SetEmailHandler(httpd_req_t *req)
    {
        char content[100] = {0};
        int ret = httpd_req_recv(req, content, sizeof(content) - 1);
        if (ret <= 0) {
            ESP_LOGE(TAG.c_str(), "Failed to receive data");
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive data");
            return ESP_FAIL;
        }
        content[ret] = '\0';

        // Configuration HTTP-client
        esp_http_client_config_t config = {};
        config.url = SERVER_URL;
        config.method = HTTP_METHOD_POST;
        config.cert_pem = (const char *)YANDEX_ROOT_CA.c_str();
        config.timeout_ms = 5000;
        config.event_handler = ProcessMqttAuthInfo;
        config.user_data = this;

        // Initializing the HTTP Client
        esp_http_client_handle_t client = esp_http_client_init(&config);

        // Setting the Content-Type header
        esp_http_client_set_header(client, "Content-Type", "application/json");

        std::string jsonRequest = "[" + std::string(content) + ", {\"device_id\":\"" + std::to_string(_deviceId) + "\"}]";

        _mqttAuthData.clear();

        // Set the request body
        esp_http_client_set_post_field(client, jsonRequest.c_str(), strlen(jsonRequest.c_str()));

        // Execute the request
        esp_err_t err = esp_http_client_perform(client);
        char message[128];

        ESP_LOGI(TAG.c_str(), "%s", _mqttAuthData.c_str());

        cJSON *root = cJSON_Parse(_mqttAuthData.c_str());
        if (root == NULL) {
            ESP_LOGE(TAG.c_str(), "Failed to JSON parse");
            esp_http_client_cleanup(client);
            cJSON_Delete(root);
            httpd_resp_send(req, "HTTP POST request failed.", HTTPD_RESP_USE_STRLEN);
            return ESP_FAIL;
        }

        // Extract firmware version or error
        cJSON *mqttLogin = cJSON_GetObjectItem(root, "MqttLogin");
        cJSON *mqttPassword = cJSON_GetObjectItem(root, "MqttPassword");

        if (mqttLogin == NULL || mqttPassword == NULL) {
            ESP_LOGE(TAG.c_str(), "Failed to JSON parse");
            esp_http_client_cleanup(client);
            cJSON_Delete(root);
            httpd_resp_send(req, "HTTP POST request failed.", HTTPD_RESP_USE_STRLEN);
            return ESP_FAIL;
        }

        ESP_LOGI(TAG.c_str(), "MqttLogin=%s, MqttPassword=%s", mqttLogin->valuestring, mqttPassword->valuestring);

        // Call only once with initialization MQTT
        _nvsMemoryManager->WriteDataToFlash("MqttLog", mqttLogin->valuestring);
        _nvsMemoryManager->WriteDataToFlash("MqttPwd", mqttPassword->valuestring);
        std::string mqttLoginString = std::string(mqttLogin->valuestring);
        std::string mqttPasswordString = std::string(mqttPassword->valuestring);
        _setMqttAuthInfoWithInitCallback(mqttLoginString, mqttPasswordString);

        if (err == ESP_OK) {
            snprintf(message, sizeof(message), "HTTP POST Status = %d", esp_http_client_get_status_code(client));
            ESP_LOGI(TAG.c_str(), "%s", message);
        } else {
            snprintf(message, sizeof(message), "HTTP POST request failed: %s", esp_err_to_name(err));
            ESP_LOGI(TAG.c_str(), "%s", message);
        }

        // Freeing up resources
        esp_http_client_cleanup(client);
        cJSON_Delete(root);

        // Send message to client
        httpd_resp_send(req, message, HTTPD_RESP_USE_STRLEN);

        return ESP_OK;
    }

    // Designed to setup configure
    void WebServerManager::Initialize()
    {
        _setMqttAuthInfoWithInitCallback = nullptr;
        _nvsMemoryManager->ReadDataFromFlash("login", _login);
        _nvsMemoryManager->ReadDataFromFlash("password", _password);
        _deviceId = 0;
    }

    // Designed to run HTTP server
    httpd_handle_t WebServerManager::StartWebServer(void)
    {
        httpd_handle_t server = NULL;
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();

        if (httpd_start(&server, &config) == ESP_OK) {

            httpd_uri_t root = {
                .uri = "/",
                .method = HTTP_GET,
                .handler = StaticRootHandler,
                .user_ctx = this
            };

            httpd_register_uri_handler(server, &root);

            httpd_uri_t setWiFiUri = {
                .uri = "/submit",
                .method = HTTP_POST,
                .handler = StaticSetWiFiHandler,
                .user_ctx = this
            };

            httpd_register_uri_handler(server, &setWiFiUri);

            httpd_uri_t calibrateUri = {
                .uri = "/calibrate",
                .method = HTTP_POST,
                .handler = StaticCalibrateHandler,
                .user_ctx = this
            };

            httpd_register_uri_handler(server, &calibrateUri);

            httpd_uri_t adjustPulsesUri = {
                .uri = "/adjustPulses",
                .method = HTTP_POST,
                .handler = StaticAdjustPulsesHandler,
                .user_ctx = this
            };

            httpd_register_uri_handler(server, &adjustPulsesUri);

            httpd_uri_t configureSleepModeUri = {
                .uri = "/configureSleepMode",
                .method = HTTP_POST,
                .handler = StaticSetSleepModeHandler,
                .user_ctx = this
            };

            httpd_register_uri_handler(server, &configureSleepModeUri);

            httpd_uri_t getCurrentTimeUri = {
                .uri = "/getCurrentTime",
                .method = HTTP_POST,
                .handler = StaticGetCurrentTimeHandler,
                .user_ctx = this
            };

            httpd_register_uri_handler(server, &getCurrentTimeUri);

            httpd_uri_t setEmailUri = {
                .uri = "/setEmail",
                .method = HTTP_POST,
                .handler = StaticSetEmailHandler,
                .user_ctx = this
            };

            httpd_register_uri_handler(server, &setEmailUri);

            httpd_uri_t startOtaUri = {
                .uri = "/startOTA",
                .method = HTTP_GET,
                .handler = StaticStartOtaHandler,
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &startOtaUri);

            httpd_uri_t getOtaProgressUri = {
                .uri = "/otaProgress",
                .method = HTTP_GET,
                .handler = StaticGetOtaProgressHandler,
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &getOtaProgressUri);

            httpd_uri_t checkFirmwareUri = {
                .uri = "/checkFirmware",
                .method = HTTP_GET,
                .handler = StaticCheckFirmwareHandler,
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &checkFirmwareUri);

            httpd_uri_t setUniqueIdUri = {
                .uri = "/setUniqueId",
                .method = HTTP_POST,
                .handler = StaticSetUniqueIdHandler,
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &setUniqueIdUri);
        }

        return server;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticSetUniqueIdHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->SetUniqueIdHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::SetUniqueIdHandler(httpd_req_t* req)
    {
        char buffer[128];
        int ret = httpd_req_recv(req, buffer, sizeof(buffer) - 1);
        if (ret <= 0) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        buffer[ret] = '\0';

        // Parse JSON
        cJSON *root = cJSON_Parse(buffer);
        if (root == NULL) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        // Extract UniqieId
        cJSON *id = cJSON_GetObjectItem(root, "id");
        if (id == NULL) {
            cJSON_Delete(root);
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        ESP_LOGI(TAG.c_str(), "Получен ID: %s", id->valuestring);
        _deviceId = std::stoi(id->valuestring);
        _nvsMemoryManager->WriteDataToFlash("UniqueId", _deviceId);

        // Send response
        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "status", "success");
        cJSON_AddStringToObject(response, "message", "Unique Id принят");
        const char *response_str = cJSON_Print(response);

        httpd_resp_set_type(req, "application/json");
        httpd_resp_send(req, response_str, HTTPD_RESP_USE_STRLEN);

        cJSON_Delete(root);
        cJSON_Delete(response);

        return ESP_OK;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticCheckFirmwareHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->CheckFirmwareHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed for send HTTP response with message
    esp_err_t WebServerManager::SendHttpResponseWithMessage(httpd_req_t* req, const std::string& message)
    {
        cJSON* root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "statusMessage", message.c_str());

        const char *response = cJSON_Print(root);
        httpd_resp_set_type(req, "application/json");
        httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);

        cJSON_Delete(root);

        return ESP_OK;
    }

    // Designed to handle HTTP event callback
    esp_err_t WebServerManager::ProcessJsonFirmwareVersion(esp_http_client_event_t *evt)
    {
        switch ((int)evt->event_id) {
            case HTTP_EVENT_ON_DATA: {
                WebServerManager* self = static_cast<WebServerManager*>(evt->user_data);
                if (self) {
                    return self->SetJsonFirmwareVersion((char *)evt->data, evt->data_len);
                }
            }    break;
            default:
                break;
        }
        return ESP_OK;
    }

    // Designed to set JSON firmware version
    esp_err_t WebServerManager::SetJsonFirmwareVersion(const char* data, const uint32_t length)
    {
        _jsonFirmwareVersion = std::string(data, length);
        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::CheckFirmwareHandler(httpd_req_t *req)
    {
        std::string statusMessage = {};

        esp_http_client_config_t config = {};
        config.url = SERVER_URL;
        config.method = HTTP_METHOD_POST;
        config.cert_pem = (const char *)YANDEX_ROOT_CA.c_str();
        config.event_handler = ProcessJsonFirmwareVersion;
        config.user_data = this;

        esp_http_client_handle_t client = esp_http_client_init(&config);

        // Setup headers
        esp_http_client_set_header(client, "Content-Type", "application/json");

        // Body of header
        const char *postData = "{\"firmwareVersion\": 1}";
        esp_http_client_set_post_field(client, postData, strlen(postData));

        // Execute the request
        esp_err_t err = esp_http_client_perform(client);
        if (err != ESP_OK) {
            esp_http_client_cleanup(client);
            statusMessage = "Ошибка при запросе.";
            return SendHttpResponseWithMessage(req, statusMessage);
        }

        // Receiver status response
        int32_t statusCode = esp_http_client_get_status_code(client);
        if (statusCode != 200) {
            esp_http_client_cleanup(client);
            statusMessage = "Ошибка сервера: " + std::to_string(statusCode) + ".";
            return SendHttpResponseWithMessage(req, statusMessage);
        }

        // Parse JSON-response
        cJSON *root = cJSON_Parse(_jsonFirmwareVersion.c_str());
        if (root == NULL) {
            esp_http_client_cleanup(client);
            statusMessage = "Ошибка парсинга JSON.";
            return SendHttpResponseWithMessage(req, statusMessage);
        }

        // Extract firmware version or error
        cJSON *serverFirmwareVersionJson = cJSON_GetObjectItem(root, "version");
        cJSON *error = cJSON_GetObjectItem(root, "error");

        ESP_LOGI("TAG", "Curr=%s, New=%s", _currentFirmwareVersion.c_str(), std::string(serverFirmwareVersionJson->valuestring).c_str());

        if (serverFirmwareVersionJson) {
            statusMessage = CompareFirmwareVersions(_currentFirmwareVersion, std::string(serverFirmwareVersionJson->valuestring));
        } else if (error) {
            statusMessage = "Ошибка: " + std::string(serverFirmwareVersionJson->valuestring) + ".";
        } else {
            statusMessage = "Неизвестный ответ сервера.";
        }

        cJSON_Delete(root);
        esp_http_client_cleanup(client);

        return SendHttpResponseWithMessage(req, statusMessage);
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticGetOtaProgressHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->GetOtaProgressHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::GetOtaProgressHandler(httpd_req_t *req)
    {
        cJSON *root = cJSON_CreateObject();
       cJSON_AddNumberToObject(root, "progress", _otaUpdater->GetOtaProgress());
       cJSON_AddStringToObject(root, "statusMessage", _otaUpdater->GetOtaStatusMessage().c_str());
       const char *response = cJSON_Print(root);
       httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
       cJSON_Delete(root);

       return ESP_OK;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticStartOtaHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->StartOtaHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::StartOtaHandler(httpd_req_t *req)
    {
        httpd_resp_send(req, "OTA начата. Подготовка к загрузке...", HTTPD_RESP_USE_STRLEN);
        _otaUpdater->OtaUpdate();

        return ESP_OK;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticSetEmailHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->SetEmailHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticRootHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->RootHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticSetWiFiHandler(httpd_req_t *req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->SetWiFiHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticCalibrateHandler(httpd_req_t *req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->CalibrateHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticAdjustPulsesHandler(httpd_req_t *req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->AdjustPulsesHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticSetSleepModeHandler(httpd_req_t *req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->SetSleepModeHandler(req);
        }
        return ESP_FAIL;
    }
    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticGetCurrentTimeHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->GetCurrentTimeHandler(req);
        }
        return ESP_FAIL;
    }

    // Designed to save device id
    void WebServerManager::SetDeviceId(const int32_t deviceId)
    {
        _deviceId = deviceId;
    }

    // Designed for set current firmware version
    void WebServerManager::SetCurrentFirmwareVersion(const std::string& currentFirmvareVersion)
    {
        _currentFirmwareVersion = currentFirmvareVersion;
    }

    // Designed for the compare firmware versions
    std::string WebServerManager::CompareFirmwareVersions(const std::string& currentVersion, const std::string& serverVersion)
    {
        // Divide versions into major and minor
        size_t currentDot = currentVersion.find('.');
        size_t serverDot = serverVersion.find('.');

        if (currentDot == std::string::npos || serverDot == std::string::npos) {
            return "Ошибка: неверный формат версии.";
        }

        // Convert major and minor to numbers
        int currentMajor = std::stoi(currentVersion.substr(0, currentDot));
        int currentMinor = std::stoi(currentVersion.substr(currentDot + 1));

        int serverMajor = std::stoi(serverVersion.substr(0, serverDot));
        int serverMinor = std::stoi(serverVersion.substr(serverDot + 1));

        // Compare versions
        if (serverMajor > currentMajor || (serverMajor == currentMajor && serverMinor > currentMinor)) {
            return "Доступно обновление. Текущая версия: " + currentVersion + ", версия на сервере: " + serverVersion + ".";
        } else {
            return "Ваша система обновлена. Текущая версия: " + currentVersion + ".";
        }
    }

}
