/*
 *  Copyright (c) 2015 - 2025 MaiKe Labs
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
#include "user_config.h"

void ICACHE_FLASH_ATTR red_led_on()
{
#ifdef DEBUG
	os_printf("led on\r\n");
	os_printf("set gpio12 to low\r\n");
#endif
	gpio_output_set(0, BIT12, BIT12, 0);
}

void ICACHE_FLASH_ATTR red_led_off()
{
#ifdef DEBUG
	os_printf("led off\r\n");
	os_printf("set gpio12 to high\r\n");
#endif
	gpio_output_set(BIT12, 0, BIT12, 0);
}

void ICACHE_FLASH_ATTR led_init()
{
	gpio_init();

	// GPIO13: the wifi status led
	wifi_status_led_install (13, PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);

	// Set GPIO12 to output mode
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
}

void ICACHE_FLASH_ATTR led_set_status(uint8_t status)
{
	if(status == 0) {
		red_led_off();
	} else if (status == 1) {
		red_led_on();
	}
}

void ICACHE_FLASH_ATTR wifi_led_enable()
{
	// GPIO13: the wifi status led
	wifi_status_led_install (13, PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
}

void ICACHE_FLASH_ATTR wifi_led_disable()
{
	wifi_status_led_uninstall();
}
