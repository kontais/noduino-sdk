/*
 *  Copyright (c) 2016 kontais@aliyun.com
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
#include "osapi.h"
#include "os_type.h"
#include "driver/uart.h"

void ICACHE_FLASH_ATTR uart_debug_init()
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000);

	UART_SetPrintPort(1);

	os_printf("\n\nSDK version:%s\n", system_get_sdk_version());
}

