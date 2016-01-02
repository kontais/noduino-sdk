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

LOCAL struct keys_param keys;
LOCAL struct single_key_param *single_key[XKEY_NUM];

LOCAL void ICACHE_FLASH_ATTR
xkey_long_press(void)
{
    system_restore();
    system_restart();
}

LOCAL void ICACHE_FLASH_ATTR
xkey_short_press(void)
{
	// reverse the status of relay
	uint8_t st = (~relay_get_status()) & 0x1;

	param_set_status(st);
	param_save();

	relay_set_status(st);
}

void ICACHE_FLASH_ATTR xkey_init()
{
	single_key[0] = key_init_single (XKEY_IO_NUM, XKEY_IO_MUX, XKEY_IO_FUNC,
									xkey_long_press, xkey_short_press);
	keys.key_num = XKEY_NUM;
	keys.single_key = single_key;
	pu_key_init(&keys);
}
