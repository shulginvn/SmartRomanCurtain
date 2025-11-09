#pragma once

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <cmath>

#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_sleep.h"
#include "esp_task_wdt.h"

#include "driver/gpio.h"

#include "NvsMemoryManager.h"

namespace SmartRomanCurtain
{
    class MotorController {
        public:
            // Constructor
            MotorController();

            // Destructor
            virtual ~MotorController();

            // Designed to setup configure
            void Initialize();

            // Designed to set instance
            void Set(NvsMemoryManager* nvsMemoryManager);

            // Designed to handle change motor state  event
            void HandleMotorStateChanged(const std::string& motorState);

            // Designed to handle change motor state  event
            void HandleMotorRangeChanged(const uint32_t range);

            // Designed to set calibration value
            void SetBaseCalibration(const int32_t baseCalibration);

            // Designed to get calibration value
            int32_t GetBaseCalibration();

            // Designed to set position in pulses
            void SetPositionInPulses(const int32_t pulses);

            // Designed to set position in pulses
            int32_t GetPositionInPulses();

            // Designed to get motor state and range values
            std::pair<bool, int32_t> GetMotorOnOffAndRange();

        private:

            enum class EnMotorState
            {
                OPENING,
                CLOSING,
                STOPPED,
                INITIALIZING
            };

            enum class EnMotorCommand
            {
                OPEN,
                CLOSE,
                STOP,
                INITIALIZE
            };

            enum class EnWorkMode
            {
                NORMAL,
                TEST
            };

            xQueueHandle _gpioEvtQueue = NULL;

            const std::string TAG = "MOTOR_CONTROLLER";
            const gpio_num_t FORWARD_REVERSE_CONTROL_GPIO = GPIO_NUM_2; // GPIO_NUM_2 XIAO
            const gpio_num_t PWMA_GPIO =  GPIO_NUM_3; // GPIO_NUM_3
            const gpio_num_t HALL_SENSOR_PIN = GPIO_NUM_4; // GPIO_NUM_4
            //const gpio_num_t MOTOR_POWER_PIN = GPIO_NUM_3;

            const int32_t PAUSE_AFTER_OPEN = /*2 * 60 */15* 1000;
            const int32_t PAUSE_AFTER_CLOSE = /*1 * 60 * 1000 */10 * 1000;
            const int64_t BREAKING_TIME_INTERVAL = 10000;
            const int64_t BREAKING_MIN_PULSE_QTY = 7;
            const int64_t PRINT_INTERVAL = 1000000;
            const double CALIBRATION_ERROR_RATIO = 1.00053;
            const float EPSILON = 0.01;

            NvsMemoryManager* _nvsMemoryManager;

            // Breaking variables
            int64_t _pulseCounter = 0;
            int64_t _lastPulseCounter = 0;
            int64_t _breakingLastTime = 0;

            // Motor manage variables
            portMUX_TYPE _motorMutex = portMUX_INITIALIZER_UNLOCKED;
            QueueHandle_t _motorCommandQueue = nullptr;
            enum EnMotorState _motorState = EnMotorState::STOPPED;
            enum EnWorkMode _workMode = EnWorkMode::NORMAL;

            float _motorPosition = 0.0f;
            int32_t _baseCalibration = 0;
            int32_t _openCloseCounter = 0;
            std::unique_ptr<float> _ratio;

            bool _motorOnOff = false;
            uint32_t _targetPosition = 0;

            // Designed to wrap IRQ handler HallSensorIsrHandler
            static void IRAM_ATTR StaticHandleHallSensorIsr(void *arg);

            // Designed to handle IRQ from GPIO
            void HandleHallSensorIsr(void *arg);

            // Designed to wrap task DoEncoderTask
            static void StaticDoEncoderTask(void *arg);

            // Designed to wrap task DoHallSensorTask
            static void StaticDoHallSensorTask(void *arg);

            // Designed to wrap task DoMotorManageTask
            static void StaticDoMotorManageTask(void *arg);

            // Designed to handle working states of the motor
            void DoEncoderTask();

            // Designed to handle pulse event
            void DoHallSensorTask();

            // Designed to handle command
            void DoMotorManageTask();

            // Designed for stop motor
            void StopMotor();

            // Designed for start motor up
            void StartMotorUp();

            // Designed for start motor down
            void StartMotorDown();

            // Designed to wait pause after open and close
            bool WaitPause(uint32_t msTarget);

            // Designed for send command to motor
            void SendMotorCommand(const EnMotorCommand& motorCommand);

            // Designed for provide emergency braking mechanism
            void CheckForceAndEmergencyStop();
    };
}
