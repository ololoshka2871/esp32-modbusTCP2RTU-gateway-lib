#ifndef _MB_GATEWAY_H_
#define _MB_GATEWAY_H_

#define MB_GATEWAY_PORT 502

#ifdef __cplusplus
extern "C" {
#endif

struct mb_gateway_config {
  //! UART port to use (0 - system log, 1 or 2 available)
  int uart_port_no;

  //! UART port speed
  int uart_speed;

  //! ESP32 pin number to use as TX
  int tx_pin_no;

  //! ESP32 pin number to use as RX
  int rx_pin_no;

  //! ESP32 pin to use RE/DE control
  int re_de_pin_no;

  //! gatewat TCP port (1-65535)
  int gateway_port;

  //! gateway maximum number of connections from clients at the same time
  int gateway_max_con;

  //! gateway maximum numbers trys to process request (>=0)
  int gateway_max_trys;

  //! gateway pause betwen requests in ms (>0)
  unsigned long gateway_pause_betwen_requests_ms;

  //! gateway serial respounce wait timeout in ms (>0)
  unsigned long gateway_serial_responce_wait_ms;

  //! gateway client connection timeout in s (>0)
  int gateway_client_connection_timeout_s;

  //! gateway log-level [0-9]
  char gateway_log_level;

  //! uart mode
  //! like 8N1 and so one
  char uart_mode[3];
};

#define MB_GATEWAY_CONFIG_DEFAULT(port, speed, tx, rx, re_de)                  \
  {                                                                            \
    port, speed, tx, rx, re_de, 502, 5, 0, 3, 10, 1, 3, { '8', 'N', '1' }      \
  }

//! configure gateway before start
void mb_gateway_configure(struct mb_gateway_config *config);

//! start gateway in separate thread
void mb_gateway_start();

//! recuest to stop gateway thread
void mb_gateway_stop();

#ifdef __cplusplus
}
#endif

#endif /* _MB_GATEWAY_H_ */
