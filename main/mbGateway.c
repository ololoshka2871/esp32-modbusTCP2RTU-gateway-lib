#include <esp_log.h>

#include <cfg.h>
#include <conn.h>
#include <errno.h>
#include <globals.h>
#include <queue.h>
#include <sig.h>
#include <sock.h>
#include <string.h>
#include <tty.h>
#ifdef LOG
#include <log.h>
#endif

#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_vfs_dev.h>

#include "mbGateway.h"

#define STR(x) #x
#define TOSTRING(x) STR(x)

// TODO: move to config
#define UART_PORT 1
#define UART_TX_PIN 4
#define UART_RX_PIN 2
#define UART_RTS_PIN 16

#define DEBUG_LVL 9
#define SERIAL_SPEED 57600
#define GATEWAY_MAX_CON 5
#define GATEWAY_MAX_TRY 0
#define GATEWAY_PAUSE_BETWEN_REQUESTS 3
#define GATEWAY_RESPOUNCE_WAIT 100
#define GATEWAY_CONNECTION_TIMEOUT_S 1

int isdaemon = TRUE;
/* Server socket */
int server_sd = -1;
/* TTY related data storage variable */
ttydata_t tty;
/* Connections queue descriptor */
queue_t queue;

static void configure() {
  isdaemon = false;
  cfg.trxcntl = TRX_RTS;
  cfg.dbglvl = DEBUG_LVL;
  *cfg.logname = '\0';
  strcpy(cfg.ttyport, "/dev/uart/" TOSTRING(UART_PORT));
  cfg.ttyspeed = SERIAL_SPEED;
  strcpy(cfg.ttymode, "8N1");
  cfg.serverport = MB_GATEWAY_PORT;
  cfg.maxconn = GATEWAY_MAX_CON;
  cfg.maxtry = GATEWAY_MAX_TRY;
  cfg.rqstpause = GATEWAY_PAUSE_BETWEN_REQUESTS;
  cfg.respwait = GATEWAY_RESPOUNCE_WAIT;
  cfg.conntimeout = GATEWAY_CONNECTION_TIMEOUT_S;
}

static void early_init_uart() {
  const uart_port_t uart_num = (uart_port_t)UART_PORT;
  uart_config_t uart_config = {
      .baud_rate = SERIAL_SPEED,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl =
          UART_HW_FLOWCTRL_DISABLE, // need to work UART_MODE_RS485_HALF_DUPLEX
  };
  // Configure UART parameters
  ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(uart_num,
                               UART_TX_PIN,  // TX
                               UART_RX_PIN,  // RX
                               UART_RTS_PIN, // RTS
                               GPIO_NUM_NC   // CTS
                               ));
  const int uart_buffer_size = (256 * 2);
  ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size,
                                      uart_buffer_size, 0, NULL, 0));

  // We have a driver now installed so set up the read/write functions to use
  // driver also.
  esp_vfs_dev_uart_use_driver(uart_num);

  // Setup UART in rs485 half duplex mode
  // control rs485 IC via RTS pin
  ESP_ERROR_CHECK(uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX));
}

void mb_gateway_main(void *p) {
  const char exename[] = "mb_gateway";

  cfg_init();

  configure();

  early_init_uart();

#ifdef LOG
  if (log_init(cfg.logname) != RC_OK) {
    return;
  }
  logw(2, "%s started...", exename);
#endif

  if (conn_init()) {
#ifdef LOG
    logw(2, "conn_init() failed, exiting...");
#endif
    return;
  }

  conn_loop();

#ifdef LOG
  logw(2, "%s exited...", exename);
#endif
}
