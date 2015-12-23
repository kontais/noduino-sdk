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
	os_printf("set gpio15 to high\n");
#endif
	gpio_output_set(BIT15, 0, BIT15, 0);
}

void relay_off()
{
#ifdef DEBUG
	os_printf("set gpio15 to low\n");
#endif
	gpio_output_set(0, BIT15, BIT15, 0);
}

void relay_init()
{
	gpio_init();

	// Set GPIO15 to output mode
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15);
}

void relay_set_status(uint8_t status)
{
	if(status == 0) {
		relay_off();
	} else if (status == 1) {
		relay_on();
	}
}

uint8_t relay_get_status()
{
	return (GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT15) == 0 ? 0 : 1;
}
