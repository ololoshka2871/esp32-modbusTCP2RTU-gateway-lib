#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

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

/* Server socket */
int server_sd = -1;
/* TTY related data storage variable */
ttydata_t tty;
/* Connections queue descriptor */
queue_t queue;

static void mb_gateway_main(void *p) {
  (void)p;

  const char exename[] = "mb_gateway";

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

void mb_gateway_configure(struct mb_gateway_config *config) {
  cfg_init();

  cfg.trxcntl = TRX_RTS;

  assert(config->gateway_log_level >= 0 && config->gateway_log_level <= 9);
  cfg.dbglvl = config->gateway_log_level;
  *cfg.logname = '\0';

  assert(config->uart_port_no >= 0 && config->uart_port_no <= 2);
  sprintf(cfg.ttyport, "/dev/uart/%d", config->uart_port_no);

  cfg.ttyspeed = config->uart_speed;

  memcpy(cfg.ttymode, config->uart_mode, sizeof(config->uart_mode));
  cfg.ttymode[sizeof(config->uart_mode)] = '\0';

  assert(config->gateway_port > 0 && config->gateway_port <= 0xFFFF);
  cfg.serverport = config->gateway_port;

  assert(config->gateway_max_con > 0);
  cfg.maxconn = config->gateway_max_con;

  assert(config->gateway_max_trys >= 0);
  cfg.maxtry = config->gateway_max_trys;

  assert(config->gateway_serial_responce_wait_ms > 0);
  cfg.rqstpause = config->gateway_pause_betwen_requests_ms;

  assert(config->gateway_serial_responce_wait_ms > 0);
  cfg.respwait = config->gateway_serial_responce_wait_ms;

  assert(config->gateway_client_connection_timeout_s > 0);
  cfg.conntimeout = config->gateway_client_connection_timeout_s;

  /*--------------------------------------------------------------------------*/

  const uart_port_t uart_num = (uart_port_t)config->uart_port_no;
  uart_config_t uart_config = {
      .baud_rate = config->uart_speed,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl =
          UART_HW_FLOWCTRL_DISABLE, // need to work UART_MODE_RS485_HALF_DUPLEX
  };
  // Configure UART parameters
  ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(uart_num,
                               config->tx_pin_no,    // TX
                               config->rx_pin_no,    // RX
                               config->re_de_pin_no, // RTS
                               GPIO_NUM_NC           // CTS
                               ));
  const int uart_buffer_size = (256 * 2);
  ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size,
                                      uart_buffer_size, 0, NULL, 0));

  // We have a driver now installed so set up the read/write functions to use
  // driver also.
  esp_vfs_dev_uart_use_driver((int)uart_num);

  // Setup UART in rs485 half duplex mode
  // control rs485 IC via RTS pin
  ESP_ERROR_CHECK(uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX));
}

void mb_gateway_start() {
  sig_init();
  xTaskCreate(mb_gateway_main, "mbusd", 10240, NULL, 10, NULL);
}

void mb_gateway_stop() { sig_flag = 1; }
