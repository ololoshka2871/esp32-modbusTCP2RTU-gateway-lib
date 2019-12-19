#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "mDNSServer.h"

#include "mbGateway.h"

#include "wifi_manager.h"

static constexpr char LOG_TAG[] = "app_main";

void register_mdns_services(void *) {
  mb_gateway_config gateway_config =

      MB_GATEWAY_CONFIG_DEFAULT(1,     // UART1
                                57600, // speed
                                4,     // TX pin
                                2,     // RX pin
                                16     // RE/DE pin
      );

  // gateway_config.gateway_log_level = 9;

  mb_gateway_configure(&gateway_config);
  mb_gateway_start();

  auto &mdns = mDNSServer::instance();
  mdns.addService("Wifi Manager", "_http", "_tcp", 80, {{"path", "/"}});
  mdns.addService("Modbus TCP2RTU Gateway", "_mbtcp", "_tcp", MB_GATEWAY_PORT);
}

extern "C" void app_main(void) {
  wifi_manager_start();
  wifi_manager_set_callback(ORDER_LOAD_AND_RESTORE_STA, register_mdns_services);
}
