
#include "MotorController.h"

namespace SmartRomanCurtain
{
    MotorController::MotorController() {
        _nvsMemoryManager = nullptr;
    }

    MotorController::~MotorController() {
        // TODO Auto-generated destructor stub
    }

    // Designed to setup configure
    void MotorController::Initialize()
    {
        gpio_reset_pin(PWMA_GPIO);
        gpio_set_direction(PWMA_GPIO, GPIO_MODE_OUTPUT);

        gpio_reset_pin(FORWARD_REVERSE_CONTROL_GPIO);
        gpio_set_direction(FORWARD_REVERSE_CONTROL_GPIO, GPIO_MODE_OUTPUT);

        // gpio_reset_pin(MOTOR_POWER_PIN);
        // gpio_set_direction(MOTOR_POWER_PIN, GPIO_MODE_OUTPUT);

        gpio_set_level(PWMA_GPIO, 1);
        gpio_set_level(FORWARD_REVERSE_CONTROL_GPIO, 1);
        // gpio_set_level(MOTOR_POWER_PIN, 0);

        // Hall sensor GPIO settings
        gpio_config_t io_conf;
        io_conf.intr_type = GPIO_INTR_NEGEDGE;
        io_conf.pin_bit_mask = (1ULL << HALL_SENSOR_PIN);
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        gpio_config(&io_conf);

        _gpioEvtQueue = xQueueCreate(10, sizeof(int));

        gpio_install_isr_service(0);
        gpio_isr_handler_add(HALL_SENSOR_PIN, StaticHallSensorIsrHandler, this);

        xTaskCreate(StaticDoHallSensorTask, "HallSensorTask", 2 * 1024, this, configMAX_PRIORITIES-1, NULL);
        xTaskCreate(StaticDoEncoderTask, "EncoderTask", 2 * 1048, this, 2, NULL);

        // Restore saved parameters
        int32_t* workMode = reinterpret_cast<int32_t*>(&_workMode);
        _nvsMemoryManager->ReadDataFromFlash("wm", workMode);
        _nvsMemoryManager->ReadDataFromFlash("bc", &_baseCalibration);
        _nvsMemoryManager->ReadDataFromFlash("occ", &_openCloseCounter);

        _motorCommandQueue = xQueueCreate(5, sizeof(EnMotorCommand));

        xTaskCreatePinnedToCore(
                StaticDoMotorManageTask,
                "ControlTask",
                4096,
                this,
                configMAX_PRIORITIES - 2,
                nullptr,
                1
            );

        SendMotorCommand(EnMotorCommand::INITIALIZE);
    }

    // Designed to handle IRQ from GPIO
    void MotorController::HallSensorIsrHandler(void *arg)
    {
        xQueueSendFromISR(_gpioEvtQueue, &HALL_SENSOR_PIN, NULL);
    }

    // Designed to wrap IRQ handler HallSensorIsrHandler
    void IRAM_ATTR MotorController::StaticHallSensorIsrHandler(void *arg)
    {
        MotorController* self = static_cast<MotorController*>(arg);
        if (self) {
            self->HallSensorIsrHandler(arg);
        }
    }

    // Designed for stop motor
    void MotorController::StopMotor()
    {
        gpio_set_level(PWMA_GPIO, 1);
        gpio_set_level(FORWARD_REVERSE_CONTROL_GPIO, 1);
        //gpio_set_level(MOTOR_POWER_PIN, 0);
    }

    // Designed for start motor up
    void MotorController::StartMotorUp()
    {
        gpio_set_level(PWMA_GPIO, 0);
        gpio_set_level(FORWARD_REVERSE_CONTROL_GPIO, 1);
        //gpio_set_level(MOTOR_POWER_PIN, 1);
    }

    // Designed for start motor down
    void MotorController::StartMotorDown()
    {
        gpio_set_level(PWMA_GPIO, 0);
        gpio_set_level(FORWARD_REVERSE_CONTROL_GPIO, 0);
        //gpio_set_level(MOTOR_POWER_PIN, 1);
    }

    // Designed for provide emergency braking mechanism
    void MotorController::CheckForceAndEmergencyStop()
    {
        int64_t now = esp_timer_get_time();
        if (now - _breakingLastTime >= BREAKING_TIME_INTERVAL) {

            if (_pulseCounter - _lastPulseCounter < BREAKING_MIN_PULSE_QTY) {
                SendMotorCommand(EnMotorCommand::STOP);
                _motorPosition = 1.0f;
                ESP_LOGI(TAG.c_str(), "Emergency braking mechanism worked: %llu; %llu\n", _pulseCounter - _lastPulseCounter, now - _breakingLastTime);
            }

            _breakingLastTime = now;
            _lastPulseCounter = _pulseCounter;
        }
    }

    // Designed to handle pulse event
    void MotorController::DoHallSensorTask()
    {
        int ioNum;

        while (1) {
            if (xQueueReceive(_gpioEvtQueue, &ioNum, portMAX_DELAY)) {

                portEXIT_CRITICAL(&_motorMutex);

                _pulseCounter++;

                 switch(_motorState) {

                     case EnMotorState::CLOSING:
                         _motorPosition -= 1.0f / _baseCalibration;
                         _motorPosition = fmax(0.0f, fmin(1.0f, _motorPosition));
                         if (_motorPosition <= 0.0f) {
                             SendMotorCommand(EnMotorCommand::STOP);
                         }
                         if (_ratio != nullptr && std::abs(*_ratio - _motorPosition) <= EPSILON) {
                             SendMotorCommand(EnMotorCommand::STOP);
                             _ratio.reset();
                         }
                         CheckForceAndEmergencyStop();
                         break;

                     case EnMotorState::OPENING:
                         _motorPosition += 1.0f / (_baseCalibration * CALIBRATION_ERROR_RATIO);
                         _motorPosition = fmax(0.0f, fmin(1.0f, _motorPosition));
                         if (_motorPosition >= 1.0f) {
                             SendMotorCommand(EnMotorCommand::STOP);
                         }
                         if (_ratio != nullptr && std::abs(*_ratio - _motorPosition) <= EPSILON) {
                              SendMotorCommand(EnMotorCommand::STOP);
                              _ratio.reset();
                          }
                         CheckForceAndEmergencyStop();
                         break;
                     case EnMotorState::INITIALIZING:
                         CheckForceAndEmergencyStop();
                         break;

                     default:
                         break;
                 }

                portEXIT_CRITICAL(&_motorMutex);
            }
        }
    }


    // Designed to handle working states of the motor
    void MotorController::DoEncoderTask()
    {
        int64_t lastTime = esp_timer_get_time();
        while (1) {
            switch (_workMode) {

                case EnWorkMode::NORMAL: {
                        // TODO implement
                    }
                    break;


                case EnWorkMode::TEST: {
                        if (_motorPosition == 1.0f) {
                            if (WaitPause(PAUSE_AFTER_OPEN)) {
                                SendMotorCommand(EnMotorCommand::CLOSE);
                            }
                        } else if (_motorPosition == 0.0f) {
                            if (WaitPause(PAUSE_AFTER_CLOSE)) {
                                SendMotorCommand(EnMotorCommand::OPEN);
                                _openCloseCounter++;
                                SaveOpenCloseCounter();
                            }
                        }
                    }
                    break;
                default:
                    break;
            }

            // Statistics
            int64_t now = esp_timer_get_time();
            if (now - lastTime >= PRINT_INTERVAL) {

                lastTime = now;

                ESP_LOGI(TAG.c_str(), "Signal receive on GPIO pulseCounter=%llu, Occ=%d, motorPosition=%g, baseCalibration=%d, workMode=%d, ratio=%g\n",
                       _pulseCounter, _openCloseCounter, _motorPosition, _baseCalibration, _workMode, _ratio != nullptr ? *_ratio : 0.0);
            }

            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

    // Designed to wrap task HallSensorIsrHandler
    void MotorController::StaticDoHallSensorTask(void *arg)
    {
        MotorController* self = static_cast<MotorController*>(arg);
        if (self) {
            self->DoHallSensorTask();
        }
    }

    // Designed to wrap task DoEncoderTask
    void MotorController::StaticDoEncoderTask(void *arg)
    {
        MotorController* self = static_cast<MotorController*>(arg);
        if (self) {
            self->DoEncoderTask();
        }
    }

    // Designed to wrap task DoMotorManageTask
    void MotorController::StaticDoMotorManageTask(void *arg)
    {
        MotorController* self = static_cast<MotorController*>(arg);
        if (self) {
            self->DoMotorManageTask();
        }
    }

    // Designed to handle command
    void MotorController::DoMotorManageTask()
    {
        EnMotorCommand motorCommand;

            while(true) {
                if(xQueueReceive(_motorCommandQueue, &motorCommand, portMAX_DELAY)) {

                    portENTER_CRITICAL(&_motorMutex);
                    switch(motorCommand) {
                        case EnMotorCommand::OPEN:
                            if(_motorState != EnMotorState::OPENING && _motorPosition < 1.0f) {
                                StartMotorUp();
                                _motorState = EnMotorState::OPENING;
                            }
                            break;

                        case EnMotorCommand::CLOSE:
                            if(_motorState != EnMotorState::CLOSING && _motorPosition > 0.0f) {
                                StartMotorDown();
                                _motorState = EnMotorState::CLOSING;
                            }
                            break;

                        case EnMotorCommand::STOP:
                            StopMotor();
                            _motorState = EnMotorState::STOPPED;
                            break;

                        case EnMotorCommand::INITIALIZE:
                            StartMotorUp();
                            _motorState = EnMotorState::INITIALIZING;
                            break;

                    }
                    _breakingLastTime = esp_timer_get_time() + 2 * BREAKING_TIME_INTERVAL;
                    portEXIT_CRITICAL(&_motorMutex);
                }
            }
    }

    // Designed to wait pause after open and close
    bool MotorController::WaitPause(uint32_t msTarget)
    {
        uint32_t msCount = 0;
        while(_workMode == EnWorkMode::TEST && msCount < msTarget) {
            vTaskDelay(pdMS_TO_TICKS(100));
            msCount += 100;
        }

        return (msCount == msTarget);
    }

    // Designed for set instance
    void MotorController::Set(NvsMemoryManager* nvsMemoryManager)
    {
        _nvsMemoryManager = nvsMemoryManager;
    }

    // Designed to save close counter
    void MotorController::SaveOpenCloseCounter()
    {
        _nvsMemoryManager->SaveDataToFlash("occ", _openCloseCounter);
    }

    // Designed to read open close counter
    void MotorController::SetOpenCloseCounter(const int32_t openCloseCounter)
    {
        _openCloseCounter = openCloseCounter;
    }

    // Designed to read open close counter
    int32_t MotorController::GetOpenCloseCounter()
    {
        return _openCloseCounter;
    }

    // Designed to set calibration value
    void MotorController::SetBaseCalibration(const int32_t baseCalibration)
    {
        _baseCalibration = baseCalibration;
    }

    // Designed to get calibration value
    int32_t MotorController::GetBaseCalibration()
    {
        return _baseCalibration;
    }

    // Designed to set position in pulses
    void MotorController::SetPositionInPulses(const int32_t pulses)
    {
        portENTER_CRITICAL(&_motorMutex);
        _motorPosition = 1.0f * pulses / _baseCalibration;
        portEXIT_CRITICAL(&_motorMutex);
    }

    // Designed to set position in pulses
    int32_t MotorController::GetPositionInPulses()
    {
        return _baseCalibration * _motorPosition;
    }

    // Designed to handle change worked mode event
    void MotorController::HandleWorkModeChanged(const int32_t workMode)
    {
        portENTER_CRITICAL(&_motorMutex);
        SendMotorCommand(EnMotorCommand::STOP);
        _workMode = static_cast<EnWorkMode>(workMode);
        portEXIT_CRITICAL(&_motorMutex);
    }

    // Designed to handle change motor state  event
    void MotorController::HandleMotorStateChanged(const std::string& motorState)
    {
        portENTER_CRITICAL(&_motorMutex);
        if (motorState == "open") {
            SendMotorCommand(EnMotorCommand::OPEN);
        } else if (motorState == "close") {
            SendMotorCommand(EnMotorCommand::CLOSE);
        } else if (motorState == "stop") {
            SendMotorCommand(EnMotorCommand::STOP);
        }
        portEXIT_CRITICAL(&_motorMutex);
    }

    // Designed to handle change motor state  event
    void MotorController::HandleMotorRangeChanged(const uint32_t range)
    {
        portENTER_CRITICAL(&_motorMutex);

        _ratio.reset(new float(1.0f * range / 100));
        if (*_ratio.get() > _motorPosition) {
            SendMotorCommand(EnMotorCommand::OPEN);
        } else {
            SendMotorCommand(EnMotorCommand::CLOSE);
        }

        portEXIT_CRITICAL(&_motorMutex);
    }

    // Designed for send command to motor
    void MotorController::SendMotorCommand(const EnMotorCommand& motorCommand)
    {
        xQueueSend(_motorCommandQueue, &motorCommand, 0);
    }
}
