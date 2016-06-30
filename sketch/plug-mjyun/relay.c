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

void relay_on()
{
#ifdef DEBUG
	INFO("set gpio4 to high\n");
#endif
	gpio_output_set(BIT4, 0, BIT4, 0);
}

void relay_off()
{
#ifdef DEBUG
	INFO("set gpio4 to low\n");
#endif
	gpio_output_set(0, BIT4, BIT4, 0);
}

void relay_init()
{
	gpio_init();

	// Set GPIO4 to output mode
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
}

void relay_set_status(uint8_t status)
{
	if(status == 0) {
		relay_off();
		led_set_status(0);
	} else if (status == 1) {
		relay_on();
		led_set_status(1);
	}
}

void relay_set_status_and_publish(uint8_t status)
{
	if(status == 0) {
		relay_off();
		led_set_status(0);
		mjyun_publishstatus("off");
	} else if (status == 1) {
		relay_on();
		led_set_status(1);
		mjyun_publishstatus("on");
	}
}

uint8_t relay_get_status()
{
	return (GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT4) == 0 ? 0 : 1;
}

void relay_publish_status()
{
	uint8_t status = relay_get_status();
	if(status == 0) {
		mjyun_publishstatus("off");
	} else if (status == 1) {
		mjyun_publishstatus("on");
	}
}
