#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "os_type.h"
#include "mem.h"
#include "gpio.h"

#include "driver/key.h"
#include "driver/uart.h"

#include "mjyun.h"

#define	DEBUG			1

#define	XKEY_NUM		1

#define XKEY_IO_MUX		PERIPHS_IO_MUX_GPIO5_U
#define XKEY_IO_NUM		5
#define XKEY_IO_FUNC	FUNC_GPIO5

// NOTICE: --- For 1024KB spi flash
// 0xFA000
#define PARAM_START_SEC		0xFA

void xkey_init();

void relay_on();
void relay_off();
void relay_init();
void relay_set_status(uint8_t status);
void relay_set_status_and_publish(uint8_t status);
uint8_t relay_get_status();
void relay_publish_status();

void param_set_status(uint8_t status);
uint8_t param_get_status(void);
void param_save(void);
void param_init();

void led_init();
void led_set_status(uint8_t status);
void wifi_led_enable();
void wifi_led_disable();

#endif
