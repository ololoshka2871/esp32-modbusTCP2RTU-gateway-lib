# ESP32 ModbusTCP to RTU gateway library

Based on [mbusd](https://github.com/3cky/mbusd) project

## Building
1. Tested on [ESP32 IDF](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/) v4.1
2. Clone this repository `git clone https://github.com/ololoshka2871/esp32-modbusTCP2RTU-gateway-lib.git`
    OR as git submodule `git submodule add https://github.com/ololoshka2871/esp32-modbusTCP2RTU-gateway-lib.git`
    to your `components` folder
3. Add dependency to library to your main module: in `main/CMakeLists.txt` add line

    ```
    idf_component_register(
                    ...
    +++             DEPENDS esp32-modbusTCP2RTU-gateway-lib
    )
    ```

4. Add code to start gateway (example)

    ```c
    #include "mbGateway.h"

    ...

    // After network up...
    mb_gateway_config gateway_config =
        MB_GATEWAY_CONFIG_DEFAULT(1,     // UART1
                                  57600, // speed
                                  4,     // TX pin
                                  2,     // RX pin
                                  16     // RE/DE pin
        );
    // also you can modify defaults in gateway_config

    mb_gateway_configure(&gateway_config); // prepare gateway to satrt
    mb_gateway_start(); // new thread will be created
    ```

5. Build, flash and run your application

## Porting problems
Esp32 VFS drivers has several limitations. So it needs some fixes to get linux
library mbusd to work:
1. UARTs represents in ESP32 VFS via `/dev/uart/<UART_number>`
2. mbusd uses ioctl() to direct control RTS pin. It's unsupported by esp32-idf VFS, so
    this function disabled at all, but insted of this I use `UART_MODE_RS485_HALF_DUPLEX`
    mode [see documentation](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/uart.html#communication-mode-selection)
3. It needs to initalise UART driver **BEFORE** access it via VFS, see `mb_gateway_configure()`
4. lwip socket [does not support buffer resize](see http://lwip.100.n7.nabble.com/Setting-TCP-socket-s-send-amp-receive-buffer-size-td10263.html),
    so, skip buffers modification
5. esp32 VFS `select()` function for files, what represents UART devices work incorrectly:
    if UART file descriptor added to `select()` wait for transmitter ready set,
    `select()` allways ends with timeout. To avoid it I create hook `my_select()`,
    what skips waiting for UART file descriptor ready to write waiting (assumes it allways ready).
6. Override mbusd's logging to accept ESP32-idf logging style see esp_log.c

## Hardware
```
      VCC ---------------+
                         |
                 +-------x-------+
      RXD <------| R             |
                 |              B|-----------<> B
      TXD ------>| D    ADM483/  |
ESP32            |      ADM3072  |     RS485 bus side
      RTS --+--->| DE            |
            |    |              A|-----------<> A
            +----| /RE           |
                 +-------x-------+
                         |
                        GND
```
You mast select ESP32 UART pins RXD, TXD and RTS in `struct mb_gateway_config`
