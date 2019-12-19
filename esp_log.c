

#include "cfg.h"
#include "log.h"

#ifdef LOG

// raise local log level to maximum
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

static const char LOG_TAG[] = "mbusd";

int log_init(char *logname) {
  esp_log_level_set(LOG_TAG, ESP_LOG_VERBOSE);
  return RC_OK;
}

int log_app(char *logname, char *string) { return 0; }

/* Put message with format FMT with errorlevel LEVEL to log file */
void logw(int level, char *fmt, ...) {
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
    ESP_LOGW(LOG_TAG, "%s", str);
    break;
  case 2:
  case 3:
  case 4:
    ESP_LOGI(LOG_TAG, "%s", str);
    break;
  case 5:
    ESP_LOGD(LOG_TAG, "%s", str);
    break;
  case 6:
  case 7:
  case 8:
  case 9:
    ESP_LOGV(LOG_TAG, "%s", str);
    break;
  }
}

#endif
