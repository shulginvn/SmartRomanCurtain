idf_component_register(SRCS "Main.cpp" "TuyaController.cpp" "WebServerManager.cpp" "NvsMemoryManager.cpp" "MotorController.cpp" "DeepSleepNtp.cpp" "YandexDialogController.cpp" "WiFiConnector.cpp" "OtaUpdater.cpp"
                    INCLUDE_DIRS ".") 
target_add_binary_data(${COMPONENT_TARGET} "rootCA.crt" BINARY)