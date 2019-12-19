#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "mDNSServer.h"

#include "mbGateway.h"

#include "wifi_manager.h"

static constexpr char LOG_TAG[] = "app_main";

void register_mdns_services(void *) {
  auto &mdns = mDNSServer::instance();
  mdns.addService("Wifi Manager", "_http", "_tcp", 80, {{"path", "/"}});

  xTaskCreate(mb_gateway_main, "mbGateway", 8092, nullptr, 10, nullptr);
  mdns.addService("Modbus TCP2RTU Gateway", "_mdtcp", "_tcp", MB_GATEWAY_PORT);
}

extern "C" void app_main(void) {
  wifi_manager_start();

  wifi_manager_set_callback(ORDER_LOAD_AND_RESTORE_STA, register_mdns_services);
}
