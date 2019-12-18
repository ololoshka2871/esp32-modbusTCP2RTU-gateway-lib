#include <esp_log.h>

#include "cfg.h"
#include "log.h"

#ifdef LOG

int log_init(char *logname) { return RC_OK; }

int log_app(char *logname, char *string) { return 0; }

/* Put message with format FMT with errorlevel LEVEL to log file */
void logw(int level, char *fmt, ...) {
  static const char LOG_TAG[] = "mbusd";

  if (level > cfg.dbglvl)
    return;

  char str[INTBUFSIZE + 1];

  va_list args;
  va_start(args, fmt);
  vsnprintf(str, INTBUFSIZE, fmt, args);
  va_end(args);

  switch (level) {
  case 0:
    ESP_LOGE(LOG_TAG, "%s", str);
    break;
  case 1:
  case 2:
  case 3:
    ESP_LOGW(LOG_TAG, "%s", str);
    break;
  case 4:
  case 5:
    ESP_LOGI(LOG_TAG, "%s", str);
    break;
  case 6:
  case 7:
    ESP_LOGD(LOG_TAG, "%s", str);
    break;
  case 8:
  case 9:
    ESP_LOGV(LOG_TAG, "%s", str);
    break;
  }
}

#endif
