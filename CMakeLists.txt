# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(IDF_TARGET esp32)

set(COMPONENTS
#    app_trace
#    app_update
#    asio
#    bootloader
#    bootloader_support
#    bt
#    coap
#    console
     cxx
#    driver
#    efuse
#    esp-tls
#    esp32
#    esp_adc_cal
#    esp_common
#    esp_eth
#    esp_event
     esp_gdbstub
#    esp_http_client
#    esp_http_server
#    esp_https_ota
#    esp_https_server
#    esp_local_ctrl
#    esp_ringbuf
#    esp_rom
#    esp_websocket_client
#    esp_wifi
#    espcoredump
     esptool_py
#    expat
#    fatfs
#    freemodbus
#    freertos
#    heap
#    idf_test
#    jsmn
#    json
#    libsodium
#    log
#    lwip
#    mbedtls
#    mdns
#    mqtt
#    newlib
#    nghttp
#    nvs_flash
#    openssl
#    partition_table
#    protobuf-c
#    protocomm
#    pthread
#    sdmmc
#    smartconfig_ack
#    soc
#    spi_flash
#    spiffs
#    tcp_transport
#    tcpip_adapter
#    ulp
#    unity
#    vfs
#    wear_levelling
#    wifi_provisioning
#    wpa_supplicant
#    xtensa

     main
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32-modbusTCP2RTU-gateway)
