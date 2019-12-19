#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "sig.h"

/* signal flag */
volatile sig_atomic_t sig_flag = 0;

/*
 * Signal handler initialization
 * Return: none
 */
void sig_init(void) { assert(sig_flag == 0); }

/*
 * Signal action execution
 */
void sig_exec(void) {
#ifdef LOG
  logw(2, "Terminated.");
#endif
  vTaskDelete(NULL);
}
