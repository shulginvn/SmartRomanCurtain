
#include "WebServerManager.h"

namespace SmartRomanCurtain
{    // Service menu
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
        " width: auto;"
        " margin: 5px 0;"
        "}"
        "button {"
        " background-color: #4CAF50; color: white; padding: 10px 15px; border: none;"
        " border-radius: 4px; cursor: pointer; font-size: 16px;"
        " min-width: 125px;"
        "}"
        "button:hover { background-color: #45a049; }"
        "</style>"

        "</head>"

        "<body>"

        "<h2>Настройка уникальных параметров устройства</h2>"
        "<div>"
        " <label>Уникальный идентификатор:</label>"
        " <input type='text' id='uniqueId' placeholder='Введите уникальный идентификатор' required><br>"
        "<button onclick=\"setUniqueId()\">Отправить</button>"
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
        "<h2>Управление шторой</h2>"
        "<div>"
        " <button onclick='controlCurtain(\"open\")'>Вверх</button>"
        " <button onclick='controlCurtain(\"close\")'>Вниз</button>"
        " <button onclick='controlCurtain(\"stop\")'>Стоп</button>"
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
        "    .then(data => alert(data.message))"
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

        "function controlCurtain(action) {"
        "    fetch('/controlCurtain', {"
        "        method: 'POST',"
        "        headers: { 'Content-Type': 'application/json' },"
        "        body: JSON.stringify({"
        "            command: action"
        "        })"
        "    })"
        "    .then(response => response.json())"
        "    .then(data => alert(data.message))"
        "    .catch(error => console.error('Error:', error));"
        "}"

        "</script>"

        "</body>"
        "</html>";

    // Normal menu
    const char *WebServerManager::_htmlForm =
        "<!DOCTYPE html>"
        "<html lang=\"ru\">"
        "<head>"
        "    <meta charset=\"UTF-8\">"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "    <title>Управление умной шторой</title>"
        "    <style>"
        "        * {"
        "            margin: 0;"
        "            padding: 0;"
        "            box-sizing: border-box;"
        "        }"
        ""
        "        body {"
        "            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;"
        "            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
        "            min-height: 100vh;"
        "            padding: 20px;"
        "            color: #333;"
        "        }"
        ""
        "        .container {"
        "            max-width: 600px;"
        "            margin: 0 auto;"
        "            background: white;"
        "            border-radius: 20px;"
        "            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);"
        "            overflow: hidden;"
        "        }"
        ""
        "        .header {"
        "            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
        "            color: white;"
        "            padding: 30px 20px;"
        "            text-align: center;"
        "        }"
        ""
        "        .header h1 {"
        "            font-size: 28px;"
        "            font-weight: 600;"
        "            margin-bottom: 10px;"
        "        }"
        ""
        "        .header p {"
        "            opacity: 0.9;"
        "            font-size: 14px;"
        "        }"
        ""
        "        .main-form {"
        "            padding: 30px 20px;"
        "        }"
        ""
        "        .form-group {"
        "            margin-bottom: 20px;"
        "        }"
        ""
        "        .form-group label {"
        "            display: block;"
        "            margin-bottom: 8px;"
        "            font-weight: 500;"
        "            color: #555;"
        "            font-size: 14px;"
        "        }"
        ""
        "        .form-group input {"
        "            width: 100%;"
        "            padding: 12px 16px;"
        "            border: 2px solid #e0e0e0;"
        "            border-radius: 10px;"
        "            font-size: 16px;"
        "            transition: all 0.3s ease;"
        "            background: #f8f9fa;"
        "        }"
        ""
        "        .form-group input:focus {"
        "            outline: none;"
        "            border-color: #667eea;"
        "            background: white;"
        "            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);"
        "        }"
        ""
        "        .form-group input::placeholder {"
        "            color: #999;"
        "        }"
        ""
        "        .btn-primary {"
        "            width: 100%;"
        "            padding: 14px;"
        "            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
        "            color: white;"
        "            border: none;"
        "            border-radius: 10px;"
        "            font-size: 16px;"
        "            font-weight: 600;"
        "            cursor: pointer;"
        "            transition: all 0.3s ease;"
        "            box-shadow: 0 4px 15px rgba(102, 126, 234, 0.4);"
        "            min-height: 48px;"
        "        }"
        ""
        "        .btn-primary:hover {"
        "            transform: translateY(-2px);"
        "            box-shadow: 0 6px 20px rgba(102, 126, 234, 0.5);"
        "        }"
        ""
        "        .btn-primary:active {"
        "            transform: translateY(0);"
        "        }"
        ""
        "        .accordion {"
        "            border-top: 1px solid #e0e0e0;"
        "        }"
        ""
        "        .accordion-item {"
        "            border-bottom: 1px solid #e0e0e0;"
        "        }"
        ""
        "        .accordion-header {"
        "            width: 100%;"
        "            padding: 18px 20px;"
        "            background: #f8f9fa;"
        "            border: none;"
        "            text-align: left;"
        "            font-size: 16px;"
        "            font-weight: 600;"
        "            color: #333;"
        "            cursor: pointer;"
        "            display: flex;"
        "            justify-content: space-between;"
        "            align-items: center;"
        "            transition: background 0.3s ease;"
        "        }"
        ""
        "        .accordion-header:hover {"
        "            background: #e9ecef;"
        "        }"
        ""
        "        .accordion-header::after {"
        "            content: '+';"
        "            font-size: 24px;"
        "            font-weight: 300;"
        "            color: #667eea;"
        "            transition: transform 0.3s ease;"
        "        }"
        ""
        "        .accordion-header.active::after {"
        "            content: '−';"
        "            transform: rotate(180deg);"
        "        }"
        ""
        "        .accordion-content {"
        "            max-height: 0;"
        "            overflow: hidden;"
        "            transition: max-height 0.3s ease;"
        "            background: white;"
        "        }"
        ""
        "        .accordion-content.active {"
        "            max-height: 2000px;"
        "        }"
        ""
        "        .accordion-body {"
        "            padding: 20px;"
        "        }"
        ""
        "        .section-title {"
        "            font-size: 18px;"
        "            font-weight: 600;"
        "            margin-bottom: 20px;"
        "            color: #333;"
        "        }"
        ""
        "        .sleep-interval {"
        "            background: #f8f9fa;"
        "            padding: 15px;"
        "            border-radius: 10px;"
        "            margin-bottom: 15px;"
        "        }"
        ""
        "        .sleep-interval-title {"
        "            font-weight: 600;"
        "            margin-bottom: 15px;"
        "            color: #555;"
        "        }"
        ""
        "        .sleep-row {"
        "            display: grid;"
        "            grid-template-columns: auto 1fr;"
        "            gap: 10px;"
        "            align-items: center;"
        "            margin-bottom: 10px;"
        "        }"
        ""
        "        .sleep-row label {"
        "            font-size: 14px;"
        "            color: #666;"
        "        }"
        ""
        "        .sleep-row input[type=\"checkbox\"] {"
        "            width: 20px;"
        "            height: 20px;"
        "            cursor: pointer;"
        "        }"
        ""
        "        .sleep-row input[type=\"number\"] {"
        "            padding: 8px 12px;"
        "            border: 2px solid #e0e0e0;"
        "            border-radius: 8px;"
        "            font-size: 14px;"
        "            width: 100%;"
        "        }"
        ""
        "        .sleep-row input[type=\"number\"]:focus {"
        "            outline: none;"
        "            border-color: #667eea;"
        "        }"
        ""
        "        .btn-secondary {"
        "            padding: 14px 20px;"
        "            background: #6c757d;"
        "            color: white;"
        "            border: none;"
        "            border-radius: 10px;"
        "            font-size: 16px;"
        "            font-weight: 500;"
        "            cursor: pointer;"
        "            transition: all 0.3s ease;"
        "            margin-right: 10px;"
        "            margin-bottom: 10px;"
        "            min-height: 48px;"
        "        }"
        ""
        "        .btn-secondary:hover {"
        "            background: #5a6268;"
        "        }"
        ""
        "        .time-display {"
        "            display: flex;"
        "            gap: 10px;"
        "            margin-bottom: 15px;"
        "        }"
        ""
        "        .time-display input {"
        "            flex: 1;"
        "            padding: 10px;"
        "            border: 2px solid #e0e0e0;"
        "            border-radius: 8px;"
        "            font-size: 14px;"
        "            background: #f8f9fa;"
        "        }"
        ""
        "        .time-display .btn-secondary {"
        "            margin-bottom: 0;"
        "        }"
        ""
        "        .progress-section {"
        "            margin-top: 15px;"
        "        }"
        ""
        "        .status-message {"
        "            padding: 12px;"
        "            background: #f8f9fa;"
        "            border-radius: 8px;"
        "            margin-bottom: 15px;"
        "            font-size: 14px;"
        "            color: #333;"
        "            min-height: 40px;"
        "        }"
        ""
        "        progress {"
        "            width: 100%;"
        "            height: 30px;"
        "            border-radius: 15px;"
        "            overflow: hidden;"
        "            background-color: #e0e0e0;"
        "            border: none;"
        "            margin-bottom: 15px;"
        "        }"
        ""
        "        progress::-webkit-progress-bar {"
        "            background-color: #e0e0e0;"
        "            border-radius: 15px;"
        "        }"
        ""
        "        progress::-webkit-progress-value {"
        "            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
        "            border-radius: 15px;"
        "        }"
        ""
        "        progress::-moz-progress-bar {"
        "            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
        "            border-radius: 15px;"
        "        }"
        ""
        "        .button-group {"
        "            display: flex;"
        "            gap: 10px;"
        "            flex-wrap: wrap;"
        "        }"
        ""
        "        .button-group button {"
        "            flex: 1;"
        "            min-width: 120px;"
        "        }"
        ""
        "        @media (max-width: 768px) {"
        "            body {"
        "                padding: 10px;"
        "            }"
        ""
        "            .container {"
        "                border-radius: 15px;"
        "            }"
        ""
        "            .header {"
        "                padding: 20px 15px;"
        "            }"
        ""
        "            .header h1 {"
        "                font-size: 24px;"
        "            }"
        ""
        "            .main-form {"
        "                padding: 20px 15px;"
        "            }"
        ""
        "            .sleep-row {"
        "                grid-template-columns: 1fr;"
        "                gap: 5px;"
        "            }"
        ""
        "            .sleep-row label {"
        "                margin-bottom: 5px;"
        "            }"
        ""
        "            .button-group {"
        "                flex-direction: column;"
        "            }"
        ""
        "            .button-group button {"
        "                width: 100%;"
        "            }"
        ""
        "            .toast {"
        "                right: 10px;"
        "                left: 10px;"
        "                min-width: auto;"
        "                max-width: none;"
        "            }"
        ""
        "            .confirm-dialog-content {"
        "                margin: 20px;"
        "                width: calc(100% - 40px);"
        "            }"
        "        }"
        ""
        "        .success-message {"
        "            padding: 12px;"
        "            background: #d4edda;"
        "            color: #155724;"
        "            border-radius: 8px;"
        "            margin-top: 15px;"
        "            font-size: 14px;"
        "            display: none;"
        "        }"
        ""
        "        .success-message.show {"
        "            display: block;"
        "        }"
        ""
        "        .subsection {"
        "            margin-bottom: 30px;"
        "            padding-bottom: 20px;"
        "            border-bottom: 1px solid #e0e0e0;"
        "        }"
        ""
        "        .subsection:last-child {"
        "            border-bottom: none;"
        "            margin-bottom: 0;"
        "            padding-bottom: 0;"
        "        }"
        ""
        "        .btn-primary:disabled,"
        "        .btn-secondary:disabled {"
        "            opacity: 0.6;"
        "            cursor: not-allowed;"
        "            transform: none !important;"
        "        }"
        ""
        "        .loading {"
        "            position: relative;"
        "            color: transparent !important;"
        "        }"
        ""
        "        .loading::after {"
        "            content: '';"
        "            position: absolute;"
        "            width: 20px;"
        "            height: 20px;"
        "            top: 50%;"
        "            left: 50%;"
        "            margin-left: -10px;"
        "            margin-top: -10px;"
        "            border: 3px solid rgba(255, 255, 255, 0.3);"
        "            border-radius: 50%;"
        "            border-top-color: white;"
        "            animation: spin 0.8s linear infinite;"
        "        }"
        ""
        "        @keyframes spin {"
        "            to { transform: rotate(360deg); }"
        "        }"
        ""
        "        .password-toggle {"
        "            position: relative;"
        "        }"
        ""
        "        .password-toggle-btn {"
        "            position: absolute;"
        "            right: 12px;"
        "            top: 50%;"
        "            transform: translateY(-50%);"
        "            background: none;"
        "            border: none;"
        "            color: #667eea;"
        "            cursor: pointer;"
        "            font-size: 18px;"
        "            padding: 5px;"
        "            z-index: 1;"
        "        }"
        ""
        "        .password-toggle-btn:hover {"
        "            opacity: 0.7;"
        "        }"
        ""
        "        .form-group.error input {"
        "            border-color: #dc3545;"
        "            background: #fff5f5;"
        "        }"
        ""
        "        .form-group .error-message {"
        "            color: #dc3545;"
        "            font-size: 12px;"
        "            margin-top: 5px;"
        "            display: none;"
        "        }"
        ""
        "        .form-group.error .error-message {"
        "            display: block;"
        "        }"
        ""
        "        .toast {"
        "            position: fixed;"
        "            bottom: 20px;"
        "            right: 20px;"
        "            background: white;"
        "            padding: 16px 20px;"
        "            border-radius: 10px;"
        "            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.15);"
        "            display: flex;"
        "            align-items: center;"
        "            gap: 12px;"
        "            min-width: 300px;"
        "            max-width: 90%;"
        "            z-index: 1000;"
        "            animation: slideIn 0.3s ease;"
        "            transform: translateX(400px);"
        "            opacity: 0;"
        "        }"
        ""
        "        .toast.show {"
        "            transform: translateX(0);"
        "            opacity: 1;"
        "        }"
        ""
        "        .toast.success {"
        "            border-left: 4px solid #28a745;"
        "        }"
        ""
        "        .toast.error {"
        "            border-left: 4px solid #dc3545;"
        "        }"
        ""
        "        .toast-icon {"
        "            font-size: 20px;"
        "        }"
        ""
        "        .toast.success .toast-icon {"
        "            color: #28a745;"
        "        }"
        ""
        "        .toast.error .toast-icon {"
        "            color: #dc3545;"
        "        }"
        ""
        "        @keyframes slideIn {"
        "            from {"
        "                transform: translateX(400px);"
        "                opacity: 0;"
        "            }"
        "            to {"
        "                transform: translateX(0);"
        "                opacity: 1;"
        "            }"
        "        }"
        ""
        "        .confirm-dialog {"
        "            position: fixed;"
        "            top: 0;"
        "            left: 0;"
        "            right: 0;"
        "            bottom: 0;"
        "            background: rgba(0, 0, 0, 0.5);"
        "            display: none;"
        "            align-items: center;"
        "            justify-content: center;"
        "            z-index: 2000;"
        "        }"
        ""
        "        .confirm-dialog.show {"
        "            display: flex;"
        "        }"
        ""
        "        .confirm-dialog-content {"
        "            background: white;"
        "            padding: 30px;"
        "            border-radius: 15px;"
        "            max-width: 400px;"
        "            width: 90%;"
        "            box-shadow: 0 10px 40px rgba(0, 0, 0, 0.2);"
        "        }"
        ""
        "        .confirm-dialog-title {"
        "            font-size: 20px;"
        "            font-weight: 600;"
        "            margin-bottom: 15px;"
        "            color: #333;"
        "        }"
        ""
        "        .confirm-dialog-message {"
        "            margin-bottom: 25px;"
        "            color: #666;"
        "            line-height: 1.5;"
        "        }"
        ""
        "        .confirm-dialog-buttons {"
        "            display: flex;"
        "            gap: 10px;"
        "            justify-content: flex-end;"
        "        }"
        ""
        "        .btn-cancel {"
        "            padding: 10px 20px;"
        "            background: #e9ecef;"
        "            color: #333;"
        "            border: none;"
        "            border-radius: 8px;"
        "            cursor: pointer;"
        "            font-size: 14px;"
        "        }"
        ""
        "        .btn-confirm {"
        "            padding: 10px 20px;"
        "            background: #dc3545;"
        "            color: white;"
        "            border: none;"
        "            border-radius: 8px;"
        "            cursor: pointer;"
        "            font-size: 14px;"
        "        }"
        ""
        "        .input-icon {"
        "            position: relative;"
        "        }"
        ""
        "        .input-icon::before {"
        "            content: '';"
        "            position: absolute;"
        "            left: 12px;"
        "            top: 50%;"
        "            transform: translateY(-50%);"
        "            width: 20px;"
        "            height: 20px;"
        "            background-size: contain;"
        "            opacity: 0.5;"
        "            pointer-events: none;"
        "        }"
        ""
        "        .input-icon input {"
        "            padding-left: 40px;"
        "        }"
        "    </style>"
        "</head>"
        "<body>"
        "    <div class=\"container\">"
        "        <div class=\"header\">"
        "            <h1>🏠 Управление умной шторой</h1>"
        "            <p>Настройка подключения и параметров</p>"
        "        </div>"
        ""
        "        <div class=\"main-form\">"
        "            <form id=\"mainConfigForm\">"
        "                <div class=\"form-group\">"
        "                    <label for=\"ssid\">Имя сети Wi-Fi (SSID)</label>"
        "                    <input type=\"text\" id=\"ssid\" name=\"ssid\" placeholder=\"Введите имя сети Wi-Fi\" required>"
        "                    <span class=\"error-message\"></span>"
        "                </div>"
        ""
        "                <div class=\"form-group\">"
        "                    <label for=\"password\">Пароль сети Wi-Fi</label>"
        "                    <div class=\"password-toggle\">"
        "                        <input type=\"password\" id=\"password\" name=\"password\" placeholder=\"Введите пароль Wi-Fi\" required>"
        "                        <button type=\"button\" class=\"password-toggle-btn\" onclick=\"togglePassword()\" aria-label=\"Показать/скрыть пароль\">👁️</button>"
        "                    </div>"
        "                    <span class=\"error-message\"></span>"
        "                </div>"
        ""
        "                <div class=\"form-group\">"
        "                    <label for=\"yandexLogin\">Логин в сервисах Yandex</label>"
        "                    <input type=\"text\" id=\"yandexLogin\" name=\"yandexLogin\" placeholder=\"Введите логин Yandex\" required>"
        "                    <span class=\"error-message\"></span>"
        "                </div>"
        ""
        "                <button type=\"button\" class=\"btn-primary\" id=\"saveConfigBtn\" onclick=\"saveMainConfig()\">"
        "                    Сохранить настройки"
        "                </button>"
        "            </form>"
        ""
        "            <div id=\"successMessage\" class=\"success-message\"></div>"
        "        </div>"
        ""
        "        <div class=\"accordion\">"
        "            <div class=\"accordion-item\">"
        "                <button class=\"accordion-header\" onclick=\"toggleAccordion(this)\">"
        "                    Дополнительные настройки"
        "                </button>"
        "                <div class=\"accordion-content\">"
        "                    <div class=\"accordion-body\">"
        "                        <div class=\"subsection\">"
        "                            <div class=\"section-title\">Настройка времени</div>"
        "                            <div class=\"time-display\">"
        "                                <input type=\"text\" id=\"currentTime\" placeholder=\"Часы и минуты\" readonly>"
        "                                <button type=\"button\" class=\"btn-secondary\" onclick=\"getCurrentTime()\">"
        "                                    Прочитать"
        "                                </button>"
        "                            </div>"
        "                        </div>"
        ""
        "                        <div class=\"subsection\">"
        "                            <div class=\"section-title\">Настройка режима сна</div>"
        "                            <form id=\"sleepForm\">"
        "                                <div class=\"sleep-interval\">"
        "                                    <div class=\"sleep-interval-title\">Интервал 1</div>"
        "                                    <div class=\"sleep-row\">"
        "                                        <label>Включить:</label>"
        "                                        <input type=\"checkbox\" id=\"enableSleepInterval1\" name=\"enableSleepInterval1\">"
        "                                    </div>"
        "                                    <div class=\"sleep-row\">"
        "                                        <label>Начало сна (часы, 0-23):</label>"
        "                                        <input type=\"number\" id=\"sleepStartHourInterval1\" name=\"sleepStartHourInterval1\" min=\"0\" max=\"23\" value=\"22\">"
        "                                    </div>"
        "                                    <div class=\"sleep-row\">"
        "                                        <label>Начало сна (минуты, 0-59):</label>"
        "                                        <input type=\"number\" id=\"sleepStartMinuteInterval1\" name=\"sleepStartMinuteInterval1\" min=\"0\" max=\"59\" value=\"0\">"
        "                                    </div>"
        "                                    <div class=\"sleep-row\">"
        "                                        <label>Длительность сна (часы):</label>"
        "                                        <input type=\"number\" id=\"sleepDurationInterval1\" name=\"sleepDurationInterval1\" min=\"1\" value=\"8\">"
        "                                    </div>"
        "                                </div>"
        ""
        "                                <div class=\"sleep-interval\">"
        "                                    <div class=\"sleep-interval-title\">Интервал 2</div>"
        "                                    <div class=\"sleep-row\">"
        "                                        <label>Включить:</label>"
        "                                        <input type=\"checkbox\" id=\"enableSleepInterval2\" name=\"enableSleepInterval2\">"
        "                                    </div>"
        "                                    <div class=\"sleep-row\">"
        "                                        <label>Начало сна (часы, 0-23):</label>"
        "                                        <input type=\"number\" id=\"sleepStartHourInterval2\" name=\"sleepStartHourInterval2\" min=\"0\" max=\"23\" value=\"22\">"
        "                                    </div>"
        "                                    <div class=\"sleep-row\">"
        "                                        <label>Начало сна (минуты, 0-59):</label>"
        "                                        <input type=\"number\" id=\"sleepStartMinuteInterval2\" name=\"sleepStartMinuteInterval2\" min=\"0\" max=\"59\" value=\"0\">"
        "                                    </div>"
        "                                    <div class=\"sleep-row\">"
        "                                        <label>Длительность сна (часы):</label>"
        "                                        <input type=\"number\" id=\"sleepDurationInterval2\" name=\"sleepDurationInterval2\" min=\"1\" value=\"8\">"
        "                                    </div>"
        "                                </div>"
        ""
        "                                <button type=\"button\" class=\"btn-secondary\" onclick=\"configureSleepMode()\">"
        "                                    Сохранить режим сна"
        "                                </button>"
        "                            </form>"
        "                        </div>"
        "                    </div>"
        "                </div>"
        "            </div>"
        ""
        "            <div class=\"accordion-item\">"
        "                <button class=\"accordion-header\" onclick=\"toggleAccordion(this)\">"
        "                    Обновление системы"
        "                </button>"
        "                <div class=\"accordion-content\">"
        "                    <div class=\"accordion-body\">"
        "                        <div class=\"section-title\">Обновление прошивки</div>"
        "                        <div class=\"progress-section\">"
        "                            <div id=\"statusMessage\" class=\"status-message\">Ожидание действий.</div>"
        "                            <progress id=\"progressBar\" value=\"0\" max=\"100\"></progress>"
        "                            <div class=\"button-group\">"
        "                                <button id=\"checkFirmwareButton\" class=\"btn-secondary\">Проверить обновления</button>"
        "                                <button id=\"updateFirmwareButton\" class=\"btn-secondary\">Запустить обновление</button>"
        "                            </div>"
        "                        </div>"
        "                    </div>"
        "                </div>"
        "            </div>"
        "        </div>"
        "    </div>"
        ""
        "    <div id=\"toast\" class=\"toast\">"
        "        <span class=\"toast-icon\"></span>"
        "        <span class=\"toast-message\"></span>"
        "    </div>"
        ""
        "    <div id=\"confirmDialog\" class=\"confirm-dialog\">"
        "        <div class=\"confirm-dialog-content\">"
        "            <div class=\"confirm-dialog-title\">Подтверждение действия</div>"
        "            <div class=\"confirm-dialog-message\" id=\"confirmMessage\"></div>"
        "            <div class=\"confirm-dialog-buttons\">"
        "                <button class=\"btn-cancel\" onclick=\"closeConfirmDialog()\">Отмена</button>"
        "                <button class=\"btn-confirm\" id=\"confirmBtn\">Подтвердить</button>"
        "            </div>"
        "        </div>"
        "    </div>"
        ""
        "    <script>"
        "        let confirmCallback = null;"
        ""
        "        function togglePassword() {"
        "            const passwordInput = document.getElementById('password');"
        "            const toggleBtn = document.querySelector('.password-toggle-btn');"
        "            if (passwordInput.type === 'password') {"
        "                passwordInput.type = 'text';"
        "                toggleBtn.textContent = '🙈';"
        "            } else {"
        "                passwordInput.type = 'password';"
        "                toggleBtn.textContent = '👁️';"
        "            }"
        "        }"
        ""
        "        function showToast(message, isSuccess = true) {"
        "            const toast = document.getElementById('toast');"
        "            const icon = toast.querySelector('.toast-icon');"
        "            const messageEl = toast.querySelector('.toast-message');"
        "            "
        "            toast.className = 'toast ' + (isSuccess ? 'success' : 'error');"
        "            icon.textContent = isSuccess ? '✓' : '✕';"
        "            messageEl.textContent = message;"
        "            "
        "            toast.classList.add('show');"
        "            "
        "            setTimeout(() => {"
        "                toast.classList.remove('show');"
        "            }, 3000);"
        "        }"
        ""
        "        function showConfirmDialog(message, callback) {"
        "            const dialog = document.getElementById('confirmDialog');"
        "            const messageEl = document.getElementById('confirmMessage');"
        "            messageEl.textContent = message;"
        "            confirmCallback = callback;"
        "            dialog.classList.add('show');"
        "        }"
        ""
        "        function closeConfirmDialog() {"
        "            const dialog = document.getElementById('confirmDialog');"
        "            dialog.classList.remove('show');"
        "            confirmCallback = null;"
        "        }"
        ""
        "        document.getElementById('confirmBtn').addEventListener('click', function() {"
        "            if (confirmCallback) {"
        "                confirmCallback();"
        "                closeConfirmDialog();"
        "            }"
        "        });"
        ""
        "        document.getElementById('confirmDialog').addEventListener('click', function(e) {"
        "            if (e.target === this) {"
        "                closeConfirmDialog();"
        "            }"
        "        });"
        ""
        "        function validateForm() {"
        "            let isValid = true;"
        "            const ssid = document.getElementById('ssid').value.trim();"
        "            const password = document.getElementById('password').value.trim();"
        "            const yandexLogin = document.getElementById('yandexLogin').value.trim();"
        ""
        "            const ssidGroup = document.getElementById('ssid').closest('.form-group');"
        "            if (!ssid || ssid.length < 2) {"
        "                ssidGroup.classList.add('error');"
        "                ssidGroup.querySelector('.error-message').textContent = 'Имя сети должно содержать минимум 2 символа';"
        "                isValid = false;"
        "            } else {"
        "                ssidGroup.classList.remove('error');"
        "            }"
        ""
        "            const passwordGroup = document.getElementById('password').closest('.form-group');"
        "            if (!password || password.length < 8) {"
        "                passwordGroup.classList.add('error');"
        "                passwordGroup.querySelector('.error-message').textContent = 'Пароль должен содержать минимум 8 символов';"
        "                isValid = false;"
        "            } else {"
        "                passwordGroup.classList.remove('error');"
        "            }"
        ""
        "            const yandexGroup = document.getElementById('yandexLogin').closest('.form-group');"
        "            if (!yandexLogin || !yandexLogin.includes('@')) {"
        "                yandexGroup.classList.add('error');"
        "                yandexGroup.querySelector('.error-message').textContent = 'Введите корректный email или логин Yandex';"
        "                isValid = false;"
        "            } else {"
        "                yandexGroup.classList.remove('error');"
        "            }"
        ""
        "            return isValid;"
        "        }"
        ""
        "        document.getElementById('ssid').addEventListener('blur', validateForm);"
        "        document.getElementById('password').addEventListener('blur', validateForm);"
        "        document.getElementById('yandexLogin').addEventListener('blur', validateForm);"
        ""
        "        function validateTime(input) {"
        "            const value = parseInt(input.value);"
        "            const min = parseInt(input.min);"
        "            const max = parseInt(input.max);"
        "            if (value < min || value > max) {"
        "                input.style.borderColor = '#dc3545';"
        "                return false;"
        "            } else {"
        "                input.style.borderColor = '#e0e0e0';"
        "                return true;"
        "            }"
        "        }"
        ""
        "        document.querySelectorAll('input[type=\"number\"][min][max]').forEach(input => {"
        "            input.addEventListener('blur', function() {"
        "                validateTime(this);"
        "            });"
        "        });"
        ""
        "        function loadSavedValues() {"
        "            const savedSsid = localStorage.getItem('curtain_ssid');"
        "            const savedPassword = localStorage.getItem('curtain_password');"
        "            const savedYandex = localStorage.getItem('curtain_yandex');"
        ""
        "            if (savedSsid && savedYandex && savedPassword) {"
        "               document.getElementById('ssid').value = savedSsid;"
        "               document.getElementById('password').value = savedPassword;"
        "               document.getElementById('yandexLogin').value = savedYandex;"
        "            } else {"
        "               loadFromBackend();"
        "            }"
        "        }"
        ""
        "        function loadFromBackend() {"
        "            fetch('/getStoredSettings', {"
        "                method: 'POST',"
        "                headers: { 'Content-Type': 'application/x-www-form-urlencoded' },"
        "                body: ``"
        "            })"
        "            .then(response => {"
        "                if (!response.ok) {"
        "                    throw new Error('Ошибка при загрузке настроек');"
        "                }"
        "                return response.json();"
        "            })"
        "            .then(data => {"
        "                document.getElementById('ssid').value = data.ssid;"
        "                document.getElementById('password').value = data.password;"
        "                document.getElementById('yandexLogin').value = data.yandexLogin;"
        "                saveToLocalStorage(data.ssid, data.password, data.yandexLogin);"
        ""
        "                showToast('Сохраненные настройки успешно загружены!', true);"
        "                showMessage('Сохраненные настройки успешно загружены!', true);"
        "            })"
        "            .catch(error => {"
        "                showToast('Не удалось загрузить сохраненные настройки', false);"
        "                showMessage('Не удалось загрузить сохраненные настройки', false);"
        "            });"
        "        }"
        ""
        "        function saveToLocalStorage(ssid, password, yandexLogin) {"
        "            localStorage.setItem('curtain_ssid', ssid);"
        "            localStorage.setItem('curtain_password', password);"
        "            localStorage.setItem('curtain_yandex', yandexLogin);"
        "        }"
        ""
        "        window.addEventListener('DOMContentLoaded', loadSavedValues);"
        ""
        "        document.getElementById('mainConfigForm').addEventListener('keypress', function(e) {"
        "            if (e.key === 'Enter') {"
        "                e.preventDefault();"
        "                saveMainConfig();"
        "            }"
        "        });"
        ""
        "        function toggleAccordion(header) {"
        "            const content = header.nextElementSibling;"
        "            const isActive = header.classList.contains('active');"
        ""
        "            document.querySelectorAll('.accordion-header').forEach(h => {"
        "                h.classList.remove('active');"
        "                h.nextElementSibling.classList.remove('active');"
        "            });"
        ""
        "            if (!isActive) {"
        "                header.classList.add('active');"
        "                content.classList.add('active');"
        "            }"
        "        }"
        ""
        "        function saveMainConfig() {"
        "            const ssid = document.getElementById('ssid').value.trim();"
        "            const password = document.getElementById('password').value.trim();"
        "            const yandexLogin = document.getElementById('yandexLogin').value.trim();"
        "            const saveBtn = document.getElementById('saveConfigBtn');"
        ""
        "            if (!validateForm()) {"
        "                showToast('Пожалуйста, исправьте ошибки в форме', false);"
        "                return;"
        "            }"
        ""
        "            saveBtn.disabled = true;"
        "            saveBtn.classList.add('loading');"
        "            saveBtn.textContent = 'Сохранение...';"
        ""
        "            fetch('/setWiFiParams', {"
        "                method: 'POST',"
        "                headers: { 'Content-Type': 'application/json' },"
        "                body: JSON.stringify({"
        "                    ssid: ssid,"
        "                    password: password"
        "                })"
        "            })"
        "            .then(response => {"
        "                if (!response.ok) {"
        "                    throw new Error('Ошибка при сохранении Wi-Fi настроек');"
        "                }"
        "                return response.json();"
        "            })"
        "            .then(data => {"
        "                return fetch('/setEmail', {"
        "                    method: 'POST',"
        "                    headers: { 'Content-Type': 'application/json' },"
        "                    body: JSON.stringify({ email: yandexLogin })"
        "                });"
        "            })"
        "            .then(response => {"
        "                if (!response.ok) {"
        "                    throw new Error('Ошибка при сохранении Yandex логина');"
        "                }"
        "                return response.text();"
        "            })"
        "            .then(data => {"
        "                saveToLocalStorage(ssid, password, yandexLogin);"
        "                showToast('Настройки успешно сохранены!', true);"
        "                showMessage('Настройки успешно сохранены!', true);"
        "            })"
        "            .catch(error => {"
        "                console.error('Error:', error);"
        "                showToast('Ошибка при сохранении настроек', false);"
        "                showMessage('Ошибка при сохранении настроек', false);"
        "            })"
        "            .finally(() => {"
        "                saveBtn.disabled = false;"
        "                saveBtn.classList.remove('loading');"
        "                saveBtn.textContent = 'Сохранить настройки';"
        "            });"
        "        }"
        ""
        "        function showMessage(message, isSuccess) {"
        "            const messageEl = document.getElementById('successMessage');"
        "            messageEl.textContent = message;"
        "            messageEl.className = 'success-message show';"
        "            if (isSuccess) {"
        "                messageEl.style.background = '#d4edda';"
        "                messageEl.style.color = '#155724';"
        "            } else {"
        "                messageEl.style.background = '#f8d7da';"
        "                messageEl.style.color = '#721c24';"
        "            }"
        "            setTimeout(() => {"
        "                messageEl.classList.remove('show');"
        "            }, 3000);"
        "        }"
        ""
        "        function configureSleepMode() {"
        "            const enableSleepInterval1 = document.getElementById('enableSleepInterval1').checked ? 1 : 0;"
        "            const sleepStartHourInterval1 = document.getElementById('sleepStartHourInterval1').value;"
        "            const sleepStartMinuteInterval1 = document.getElementById('sleepStartMinuteInterval1').value;"
        "            const sleepDurationInterval1 = document.getElementById('sleepDurationInterval1').value;"
        "            const enableSleepInterval2 = document.getElementById('enableSleepInterval2').checked ? 1 : 0;"
        "            const sleepStartHourInterval2 = document.getElementById('sleepStartHourInterval2').value;"
        "            const sleepStartMinuteInterval2 = document.getElementById('sleepStartMinuteInterval2').value;"
        "            const sleepDurationInterval2 = document.getElementById('sleepDurationInterval2').value;"
        ""
        "            const hour1 = parseInt(sleepStartHourInterval1);"
        "            const minute1 = parseInt(sleepStartMinuteInterval1);"
        "            const hour2 = parseInt(sleepStartHourInterval2);"
        "            const minute2 = parseInt(sleepStartMinuteInterval2);"
        ""
        "            if (hour1 < 0 || hour1 > 23 || minute1 < 0 || minute1 > 59) {"
        "                showToast('Некорректное время для интервала 1', false);"
        "                return;"
        "            }"
        ""
        "            if (hour2 < 0 || hour2 > 23 || minute2 < 0 || minute2 > 59) {"
        "                showToast('Некорректное время для интервала 2', false);"
        "                return;"
        "            }"
        ""
        "            const btn = document.querySelector('#sleepForm button[onclick=\"configureSleepMode()\"]');"
        "            btn.disabled = true;"
        "            btn.classList.add('loading');"
        "            const originalText = btn.textContent;"
        "            btn.textContent = 'Сохранение...';"
        ""
        "            fetch('/configureSleepMode', {"
        "                method: 'POST',"
        "                headers: { 'Content-Type': 'application/x-www-form-urlencoded' },"
        "                body: `enableSleepInterval1=${enableSleepInterval1}&sleepStartHourInterval1=${sleepStartHourInterval1}&sleepStartMinuteInterval1=${sleepStartMinuteInterval1}&sleepDurationInterval1=${sleepDurationInterval1}&enableSleepInterval2=${enableSleepInterval2}&sleepStartHourInterval2=${sleepStartHourInterval2}&sleepStartMinuteInterval2=${sleepStartMinuteInterval2}&sleepDurationInterval2=${sleepDurationInterval2}`"
        "            })"
        "            .then(response => response.text())"
        "            .then(data => {"
        "                showToast('Режим сна успешно сохранен', true);"
        "            })"
        "            .catch(error => {"
        "                console.error('Error:', error);"
        "                showToast('Ошибка при сохранении режима сна', false);"
        "            })"
        "            .finally(() => {"
        "                btn.disabled = false;"
        "                btn.classList.remove('loading');"
        "                btn.textContent = originalText;"
        "            });"
        "        }"
        ""
        "        function getCurrentTime() {"
        "            const btn = document.querySelector('.time-display button[onclick=\"getCurrentTime()\"]');"
        "            btn.disabled = true;"
        "            const originalText = btn.textContent;"
        "            btn.textContent = 'Загрузка...';"
        ""
        "            fetch('/getCurrentTime', {"
        "                method: 'POST',"
        "                headers: { 'Content-Type': 'application/x-www-form-urlencoded' },"
        "                body: ``"
        "            })"
        "            .then(response => response.text())"
        "            .then(data => {"
        "                document.getElementById('currentTime').value = data;"
        "                showToast('Время успешно получено', true);"
        "            })"
        "            .catch(error => {"
        "                console.error('Error:', error);"
        "                showToast('Ошибка при получении времени', false);"
        "            })"
        "            .finally(() => {"
        "                btn.disabled = false;"
        "                btn.textContent = originalText;"
        "            });"
        "        }"
        ""
        "        document.getElementById('updateFirmwareButton').addEventListener('click', function() {"
        "            showConfirmDialog("
        "                'Вы уверены, что хотите запустить обновление прошивки? Устройство будет недоступно во время обновления.',"
        "                function() {"
        "                    const btn = document.getElementById('updateFirmwareButton');"
        "                    btn.disabled = true;"
        "                    btn.textContent = 'Обновление...';"
        ""
        "                    fetch('/startOTA')"
        "                        .then(response => {"
        "                            if (!response.ok) {"
        "                                throw new Error('Ошибка сети.');"
        "                            }"
        "                            return response.text();"
        "                        })"
        "                        .then(data => {"
        "                            document.getElementById('statusMessage').textContent = data;"
        "                            showToast('Обновление запущено', true);"
        "                            checkOtaProgress();"
        "                        })"
        "                        .catch(error => {"
        "                            console.error('Ошибка:', error);"
        "                            document.getElementById('statusMessage').textContent = 'Ошибка при запросе.';"
        "                            showToast('Ошибка при запуске обновления', false);"
        "                            btn.disabled = false;"
        "                            btn.textContent = 'Запустить обновление';"
        "                        });"
        "                }"
        "            );"
        "        });"
        ""
        "        function checkOtaProgress() {"
        "            fetch('/otaProgress')"
        "                .then(response => response.json())"
        "                .then(data => {"
        "                    document.getElementById('progressBar').value = data.progress;"
        "                    document.getElementById('statusMessage').textContent = data.statusMessage;"
        "                    if (data.progress < 100 && data.statusMessage !== 'Прошивка обновлена.') {"
        "                        setTimeout(checkOtaProgress, 100);"
        "                    }"
        "                })"
        "                .catch(error => {"
        "                    console.error('Ошибка:', error);"
        "                });"
        "        }"
        ""
        "        document.getElementById('checkFirmwareButton').addEventListener('click', function() {"
        "            const btn = this;"
        "            btn.disabled = true;"
        "            const originalText = btn.textContent;"
        "            btn.textContent = 'Проверка...';"
        ""
        "            fetch('/checkFirmware')"
        "                .then(response => response.json())"
        "                .then(data => {"
        "                    document.getElementById('statusMessage').textContent = data.statusMessage;"
        "                    showToast(data.statusMessage, true);"
        "                })"
        "                .catch(error => {"
        "                    console.error('Ошибка:', error);"
        "                    showToast('Ошибка при проверке обновлений', false);"
        "                })"
        "                .finally(() => {"
        "                    btn.disabled = false;"
        "                    btn.textContent = originalText;"
        "                });"
        "        });"
        "    </script>"
        "</body>"
        "</html>";

    // Designed to setup configure
    void WebServerManager::Initialize()
    {
        _setMqttAuthInfoWithInitCallback = nullptr;
        _nvsMemoryManager->ReadDataFromFlash("login", _login);
        _nvsMemoryManager->ReadDataFromFlash("password", _password);
        _deviceId = 0;
    }

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

    // Designed to set callback
    void WebServerManager::Set(const std::function<void(const std::string&)>& changeMotorStateCallback)
    {
        _changeMotorStateCallback = changeMotorStateCallback;
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
                .uri = "/setWiFiParams",
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

            httpd_uri_t controlCurtainUri = {
                .uri = "/controlCurtain",
                .method = HTTP_POST,
                .handler = StaticControlCurtainHandler,
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &controlCurtainUri);

            httpd_uri_t getStoredSettings = {
                .uri = "/getStoredSettings",
                .method = HTTP_POST,
                .handler = [](httpd_req_t *req) {
                    return static_cast<WebServerManager*>(req->user_ctx)->GetStoredSettingsHandler(req);
                },
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &getStoredSettings);
        }

        xTaskCreate(StaticDoSendAuthEmailTask, "SendAuthEmailTask", 4096, this, 1, NULL);

        return server;
    }

    // Designed for load stored settings
    esp_err_t WebServerManager::GetStoredSettingsHandler(httpd_req_t *req)
    {
        _nvsMemoryManager->ReadDataFromFlash("login", _readNvsBuffer);
        std::string savedSsid = std::string(_readNvsBuffer);
        _nvsMemoryManager->ReadDataFromFlash("password", _readNvsBuffer);
        std::string savedPassword = std::string(_readNvsBuffer);
        _nvsMemoryManager->ReadDataFromFlash("emailAuth", _readNvsBuffer);
        std::string savedYandex = std::string(_readNvsBuffer);

        cJSON *parsed = cJSON_Parse(savedYandex.c_str());
        cJSON *emailItem = cJSON_GetObjectItem(cJSON_GetArrayItem(parsed, 0), "email");

        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "ssid", savedSsid.c_str());
        cJSON_AddStringToObject(root, "password", savedPassword.c_str());
        cJSON_AddStringToObject(root, "yandexLogin", emailItem->valuestring);

        char *json_str = cJSON_Print(root);
        httpd_resp_set_type(req, "application/json");
        httpd_resp_send(req, json_str, HTTPD_RESP_USE_STRLEN);

        free(json_str);
        cJSON_Delete(root);
        cJSON_Delete(parsed);

        return ESP_OK;
    }

    // Designed to wrap task DoSendAuthEmailTask
    void WebServerManager::StaticDoSendAuthEmailTask(void *arg)
    {
        WebServerManager* self = static_cast<WebServerManager*>(arg);
        if (self) {
            while(!self->DoSendAuthEmailTask());
        }
        vTaskDelete(NULL);
    }

    // Designed to send authenticate email to server
    bool WebServerManager::DoSendAuthEmailTask()
    {
        // Read from NVS
        char bufferEmailAuthReq[128] = { };
        _nvsMemoryManager->ReadDataFromFlash("emailAuth", bufferEmailAuthReq);

        std::string emailAuthReq = std::string(bufferEmailAuthReq);

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

        _mqttAuthData.clear();

        // Set the request body
        esp_http_client_set_post_field(client, emailAuthReq.c_str(), strlen(emailAuthReq.c_str()));

        // Execute the request
        esp_err_t err = esp_http_client_perform(client);

        cJSON *root = cJSON_Parse(_mqttAuthData.c_str());
        if (root == NULL) {
            ESP_LOGE(TAG.c_str(), "Failed to JSON parse");
            esp_http_client_cleanup(client);
            cJSON_Delete(root);
            vTaskDelay(pdMS_TO_TICKS(1000));
            return false;
        }

        // Extract firmware version or error
        cJSON *mqttLogin = cJSON_GetObjectItem(root, "MqttLogin");
        cJSON *mqttPassword = cJSON_GetObjectItem(root, "MqttPassword");

        if (mqttLogin == NULL || mqttPassword == NULL) {
            ESP_LOGE(TAG.c_str(), "Failed to JSON parse");
            esp_http_client_cleanup(client);
            cJSON_Delete(root);
            vTaskDelay(pdMS_TO_TICKS(1000));
            return false;
        }

        ESP_LOGI(TAG.c_str(), "MqttLogin=%s, MqttPassword=%s", mqttLogin->valuestring, mqttPassword->valuestring);

        // Call only once with initialization MQTT
        _nvsMemoryManager->WriteDataToFlash("MqttLog", mqttLogin->valuestring);
        _nvsMemoryManager->WriteDataToFlash("MqttPwd", mqttPassword->valuestring);
        std::string mqttLoginString = std::string(mqttLogin->valuestring);
        std::string mqttPasswordString = std::string(mqttPassword->valuestring);
        _setMqttAuthInfoWithInitCallback(mqttLoginString, mqttPasswordString);

        int32_t httpStatus = 0;
        char message[128];

        if (err == ESP_OK) {
            static const std::map<int32_t, const char*> http_status_messages = {
                {200, "Данные получены сервером"},
                {201, "Сервер создал новый ресурс"},
                {202, "Запрос принят на обработку"},
                {204, "Нет содержимого для возврата"}
            };

            httpStatus = esp_http_client_get_status_code(client);
            auto it = http_status_messages.find(httpStatus);
            const char* statusMsg = (it != http_status_messages.end())
                ? it->second
                : "Код ответа сервера";

            snprintf(message, sizeof(message), "%s (HTTP %d).", statusMsg, httpStatus);
            ESP_LOGI(TAG.c_str(), "Статус: %s.", message);
        } else {
            static const std::map<esp_err_t, const char*> error_messages = {
                {ESP_ERR_HTTP_BASE,        "Базовая HTTP-ошибка"},
                {ESP_ERR_HTTP_CONNECT,     "Ошибка подключения"},
                {ESP_ERR_HTTP_CONNECTION_CLOSED, "Соединение закрыто"}
            };

            auto it = error_messages.find(err);
            const char* errordDesc = (it != error_messages.end())
                ? it->second
                : "Неизвестная ошибка";

            snprintf(message, sizeof(message), "%s (%s 0x%x).",
                     errordDesc, esp_err_to_name(err), err);
            ESP_LOGE(TAG.c_str(), "Ошибка: %s.", message);
        }

        // Freeing up resources
        esp_http_client_cleanup(client);
        cJSON_Delete(root);

        return (httpStatus == 200);
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::RootHandler(httpd_req_t *req)
    {
        httpd_resp_set_type(req, "text/html");

        char query[100];
        if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {

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
        char buffer[256];
        int32_t ret = httpd_req_recv(req, buffer, sizeof(buffer) - 1);
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

        // Extract parameters
        cJSON *ssid = cJSON_GetObjectItem(root, "ssid");
        cJSON *password = cJSON_GetObjectItem(root, "password");

        if (!cJSON_IsString(ssid) || !cJSON_IsString(password)) {
            cJSON_Delete(root);
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        ESP_LOGI(TAG.c_str(), "Received parameters Wi-Fi: SSID=%s, Password=%s", ssid->valuestring, password->valuestring);

        _nvsMemoryManager->WriteDataToFlash("login", ssid->valuestring);
        _nvsMemoryManager->WriteDataToFlash("password", password->valuestring);

        // Send resonse
        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "message", "Параметры Wi-Fi сохранены. Устройство будет автоматически презагружено.");
        const char *responseString = cJSON_Print(response);

        httpd_resp_send(req, responseString, HTTPD_RESP_USE_STRLEN);

        cJSON_Delete(root);
        cJSON_Delete(response);

        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::CalibrateHandler(httpd_req_t *req)
    {
        char buf[128] = {0};
        int32_t ret = httpd_req_recv(req, buf, std::min(req->content_len, sizeof(buf) - 1));
        if (ret <= 0) {
            ESP_LOGE(TAG.c_str(), "Failed to receive data");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        buf[ret] = '\0';

        char action[10] = {0}, direction[10] = {0};
        char value_str[10] = {0};
        int32_t value = 0;

        if (httpd_query_key_value(buf, "action", action, sizeof(action)) != ESP_OK) {
            ESP_LOGE(TAG.c_str(), "Missing or invalid 'action'");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        if (httpd_query_key_value(buf, "direction", direction, sizeof(direction)) != ESP_OK) {
            ESP_LOGE(TAG.c_str(), "Missing or invalid 'direction'");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        if (httpd_query_key_value(buf, "value", value_str, sizeof(value_str)) == ESP_OK) {
            value = atoi(value_str);
        }

        ESP_LOGI(TAG.c_str(), "Received action: %s, direction: %s, value: %d", action, direction, value);

        if (strcmp(action, "read") == 0) {
            int32_t calibrationValue = _motorController->GetBaseCalibration();
            char response[32];
            snprintf(response, sizeof(response), "%d", calibrationValue);
            httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
        } else if (strcmp(action, "write") == 0) {
            if (strcmp(direction, "up") == 0) {
                _motorController->SetBaseCalibration(value);
            }
            httpd_resp_send(req, "Калибровочное значение обновлено.", HTTPD_RESP_USE_STRLEN);
        } else if (strcmp(action, "save") == 0) {
            if (strcmp(direction, "up") == 0) {
                _nvsMemoryManager->WriteDataToFlash("bc", _motorController->GetBaseCalibration());
            }
            httpd_resp_send(req, "Калибровочное значение сохранено.", HTTPD_RESP_USE_STRLEN);
        } else {
            ESP_LOGE(TAG.c_str(), "Invalid action: %s", action);
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::AdjustPulsesHandler(httpd_req_t *req)
    {
        char buf[100] = {0};
        int32_t ret = httpd_req_recv(req, buf, std::min(req->content_len, sizeof(buf) - 1));

        if (ret <= 0) {
            ESP_LOGE(TAG.c_str(), "Failed to receive data");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        buf[ret] = '\0';
        ESP_LOGI(TAG.c_str(), "Received data: %s", buf);

        char action[16] = {0};
        char direction[16] = {0};
        int32_t value = 0;

        if (sscanf(buf, "action=%15[^&]&direction=%15[^&]&value=%d", action, direction, &value) < 2) {
            ESP_LOGE(TAG.c_str(), "Invalid parameters");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        if (strcmp(action, "read") == 0) {
            if (strcmp(direction, "up") == 0) {
                char response[16];
                snprintf(response, sizeof(response), "%d", _motorController->GetPositionInPulses());
                ESP_LOGI(TAG.c_str(), "Read pulses up: %d", _motorController->GetPositionInPulses());
                httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
            } else {
                ESP_LOGE(TAG.c_str(), "Invalid direction");
                httpd_resp_send_500(req);
                return ESP_FAIL;
            }
        } else if (strcmp(action, "write") == 0) {
            if (strcmp(direction, "up") == 0) {
                _motorController->SetPositionInPulses(value);
                ESP_LOGI(TAG.c_str(), "Updated pulses up: %d", _motorController->GetPositionInPulses());
                httpd_resp_send(req, "Текущее значение импульсов обновлено.", HTTPD_RESP_USE_STRLEN);
            } else {
                ESP_LOGE(TAG.c_str(), "Invalid direction");
                httpd_resp_send_500(req);
                return ESP_FAIL;
            }
        } else {
            ESP_LOGE(TAG.c_str(), "Invalid action");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::SetSleepModeHandler(httpd_req_t *req)
    {
        char content[400] = {0};
        int32_t ret = httpd_req_recv(req, content, sizeof(content) - 1);
        if (ret <= 0) {
            ESP_LOGE(TAG.c_str(), "Failed to receive data");
            httpd_resp_send_500(req);
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

        httpd_resp_send(req, "Настройки режима сна сохранены.", HTTPD_RESP_USE_STRLEN);

        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::GetCurrentTimeHandler(httpd_req_t *req)
    {
        char response[16];
        snprintf(response, sizeof(response), "%02d:%02d", _deepSleepNtp->GetCurrentHour(), _deepSleepNtp->GetCurrentMinute());
        httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);

        return ESP_OK;
    }

    // Designed to handle HTTP event callback
    esp_err_t WebServerManager::ProcessMqttAuthInfo(esp_http_client_event_t *evt)
    {
        switch ((int32_t)evt->event_id) {
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
        int32_t ret = httpd_req_recv(req, content, sizeof(content) - 1);
        if (ret <= 0) {
            ESP_LOGE(TAG.c_str(), "Failed to receive data");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        content[ret] = '\0';

        std::string jsonRequest = "[" + std::string(content) + ", {\"device_id\":\"" + std::to_string(_deviceId) + "\"}]";

        _nvsMemoryManager->WriteDataToFlash("emailAuth", jsonRequest.c_str());

        ESP_LOGE(TAG.c_str(), "TRY WRITE SIZE=%d", jsonRequest.size());

        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "message", "Настройки сохранены. Устройство будет автоматически презагружено.");
        const char *responseString = cJSON_Print(response);

        auto espResult = httpd_resp_send(req, responseString, HTTPD_RESP_USE_STRLEN);

        vTaskDelay(1000);

        esp_restart();

        return espResult;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::StartOtaHandler(httpd_req_t *req)
    {
        httpd_resp_send(req, "OTA начата. Подготовка к загрузке...", HTTPD_RESP_USE_STRLEN);
        _otaUpdater->OtaUpdate();

        return ESP_OK;
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

    // Designed for send HTTP response with message
    esp_err_t WebServerManager::SendHttpResponseWithMessage(httpd_req_t* req, const std::string& message)
    {
        cJSON* root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "statusMessage", message.c_str());

        const char *response = cJSON_Print(root);
        httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);

        cJSON_Delete(root);

        return ESP_OK;
    }

    // Designed to handle HTTP event callback
    esp_err_t WebServerManager::ProcessJsonFirmwareVersion(esp_http_client_event_t *evt)
    {
        switch ((int32_t)evt->event_id) {
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

    // Designed to handle HTTP request
    esp_err_t WebServerManager::SetUniqueIdHandler(httpd_req_t* req)
    {
        char buffer[128];
        int32_t ret = httpd_req_recv(req, buffer, sizeof(buffer) - 1);
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
        if (id == NULL || strlen(id->valuestring) <= 0) {
            cJSON_Delete(root);
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        ESP_LOGI(TAG.c_str(), "Received  ID: %s", id->valuestring);
        _deviceId = std::stoi(id->valuestring);
        _nvsMemoryManager->WriteDataToFlash("UniqueId", _deviceId);

        // Send response
        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "message", "Уникальный идентификатор сохранен.");
        const char *response_str = cJSON_Print(response);

        httpd_resp_send(req, response_str, HTTPD_RESP_USE_STRLEN);

        cJSON_Delete(root);
        cJSON_Delete(response);

        return ESP_OK;
    }

    // Designed to handle HTTP request
    esp_err_t WebServerManager::ControlCurtainHandler(httpd_req_t* req)
    {
        char buffer[64];
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

        // Extract command
        cJSON *command = cJSON_GetObjectItem(root, "command");
        if (!cJSON_IsString(command)) {
            cJSON_Delete(root);
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        const char* cmd = command->valuestring;
        ESP_LOGI(TAG.c_str(), "Curtain control command received: %s.", cmd);

        // Command processing
        static const std::unordered_map<std::string, std::string> command_map = {
            {"open",  "Открыть"},
            {"close", "Закрыть"},
            {"stop",  "Стоп"}
        };

        // Sending a reply
        cJSON *response = cJSON_CreateObject();
        auto it = command_map.find(cmd);
        if (it != command_map.end()) {
            // Executing a command
            _changeMotorStateCallback(cmd);
            cJSON_AddStringToObject(response, "status", "success");
            cJSON_AddStringToObject(response, "message", ("Команда '" + it->second + "' выполнена.").c_str());
        } else {
            cJSON_AddStringToObject(response, "status", "error");
            cJSON_AddStringToObject(response, "message", "Ошибка. Неизвестная команда.");
        }

        const char *response_str = cJSON_Print(response);
        httpd_resp_set_type(req, "application/json");
        httpd_resp_send(req, response_str, HTTPD_RESP_USE_STRLEN);

        cJSON_Delete(root);
        cJSON_Delete(response);

        return ESP_OK;
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
    esp_err_t WebServerManager::StaticStartOtaHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->StartOtaHandler(req);
        }
        return ESP_FAIL;
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

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticCheckFirmwareHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->CheckFirmwareHandler(req);
        }
        return ESP_FAIL;
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

    // Designed to handle HTTP request with wrap in static method
    esp_err_t WebServerManager::StaticControlCurtainHandler(httpd_req_t* req)
    {
        WebServerManager* self = static_cast<WebServerManager*>(req->user_ctx);
        if (self) {
            return self->ControlCurtainHandler(req);
        }
        return ESP_FAIL;
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
        int32_t currentMajor = std::stoi(currentVersion.substr(0, currentDot));
        int32_t currentMinor = std::stoi(currentVersion.substr(currentDot + 1));

        int32_t serverMajor = std::stoi(serverVersion.substr(0, serverDot));
        int32_t serverMinor = std::stoi(serverVersion.substr(serverDot + 1));

        // Compare versions
        if (serverMajor > currentMajor || (serverMajor == currentMajor && serverMinor > currentMinor)) {
            return "Доступно обновление. Текущая версия: " + currentVersion + ", версия на сервере: " + serverVersion + ".";
        } else {
            return "Ваша система обновлена. Текущая версия: " + currentVersion + ".";
        }
    }

}
