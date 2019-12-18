#include <esp_log.h>

#include <mdns.h>

#include "mDNSServer.h"

static constexpr char mDNSServerName[] = "mdns0";
static constexpr char mDNSHostName[] = CONFIG_DEFAULT_AP_SSID;

mDNSServer *mDNSServer::inst;

mDNSServer &mDNSServer::instance() {
  if (!inst) {
    inst = new mDNSServer;
  }
  return *inst;
}

mDNSServer &
mDNSServer::addService(const char *service_name, const char *service,
                       const char *proto, uint16_t port,
                       const std::vector<mdns_txt_item_t> &txtData) {
  ESP_ERROR_CHECK(mdns_service_add(
      service_name, service, proto, port,
      const_cast<mdns_txt_item_t *>(txtData.data()), txtData.size()));
  ESP_LOGI(mDNSServerName, "Registred mDNC service %s, port: %u", service_name,
           port);
  return *this;
}

mDNSServer &mDNSServer::removeService(const char *service_name,
                                      const char *proto) {
  ESP_ERROR_CHECK(mdns_service_remove(service_name, proto));
  ESP_LOGI(mDNSServerName, "Unregistred mDNC service %s", service_name);
  return *this;
}

mDNSServer::mDNSServer() {
  ESP_ERROR_CHECK(mdns_init());
  ESP_ERROR_CHECK(mdns_hostname_set(mDNSHostName));
  ESP_ERROR_CHECK(mdns_instance_name_set(mDNSServerName));
}
