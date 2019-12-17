#include <ctime>
#include <sstream>
#include <vector>

#include <driver/gpio.h>
#include <esp_log.h>

#include "i2cdev.h"

#include "GUI/GUI.h"
#include "Nixie.h"
#include "RTC.h"

#include "VSensors.h"
#include "monitor.h"

#include "sensors/CO2Sensor.h"
#include "sensors/TemperatureSensor.h"

//#include "servers/HttpServer.h"
#include "servers/VirtuinoJsonServer.h"
#include "servers/mDNSServer.h"

#include "wifi_manager.h"

#include "FastLED.h"

//#define TEST_MODE

#ifndef TEST_MODE
static TemperatureSensor ds18b20{GPIO_NUM_4, std::chrono::minutes(1)};
static CRGB leds[6];
static CO2Sensor mhz_19{UART_NUM_2, GPIO_NUM_16, GPIO_NUM_17};
static VSensors voltage_sensors{
    VSensChanel{ADC1_CHANNEL_3, 4.7f, 1.0f},  // 3.3v
    VSensChanel{ADC1_CHANNEL_7, 9.1f, 1.0f},  // 5 v
    VSensChanel{ADC1_CHANNEL_6, 24.0f, 1.0f}, // 12 v
};
#endif

static constexpr char LOG_TAG[] = "app_main";

extern "C" void app_main(void) {
#if 0
  auto monitor = new esp::Monitor;
  monitor->Start();
#endif

#if 1
  gpio_install_isr_service(0); // interrupt for all gpio events
  ESP_ERROR_CHECK(i2cdev_init());

#ifndef TEST_MODE
  RTCManager::instance()->loadTime().begin();
#endif

#ifndef TEST_MODE
  ds18b20.begin();
  mhz_19.begin();
#endif

  // turn off GPIO logging
  esp_log_level_set("gpio", ESP_LOG_NONE);

#ifndef TEST_MODE
  FastLED.addLeds<NEOPIXEL, GPIO_NUM_15>(leds, std::size(leds));
#endif

#ifndef TEST_MODE
  for (size_t i = 0; i < 3; ++i) {
    auto val = voltage_sensors.getChannelVoltage(i);
    ESP_LOGI("Voltage", "Channel %d voltage: %f V", i, val);
  }
#endif

#ifndef TEST_MODE
  GUI::init(&mhz_19, &ds18b20);
  GUI::start();
#endif

#endif

#if 1
  wifi_manager_start();

  wifi_manager_set_callback(ORDER_LOAD_AND_RESTORE_STA, [](void *) {
    mDNSServer::instance().addService("Wifi Manager", "_http", "_tcp", 80,
                                      {{"path", "/"}});

#if 0
  HttpServer::begin();
  HttpServer::start();
#endif

#if 1
    VirtuinoJsonServer::start();
#endif
  });

#endif
}
