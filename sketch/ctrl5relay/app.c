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

ctrl_status_t ctrl_st;

irom void param_save(void)
{
	INFO("Parameters are saved to Flash !\r\n");
	spi_flash_erase_sector(PARAM_START_SEC + 0);
	spi_flash_write((PARAM_START_SEC + 0) * SPI_FLASH_SEC_SIZE,
					(uint32 *) & ctrl_st,
					sizeof(ctrl_status_t));
}

irom void param_load()
{
	spi_flash_read((PARAM_START_SEC + 0) * SPI_FLASH_SEC_SIZE,
		       (uint32 *) & ctrl_st,
		       sizeof(ctrl_status_t));

	// init data of spi flash
	if (ctrl_st.init_flag == 0xff) {
		INFO("Invalid parameters, reset to default!\n");
		os_memset(&ctrl_st, 0, sizeof(ctrl_status_t));

		ctrl_st.init_flag = 1; 
		ctrl_st.start_count++;
		param_save();
	}
}

irom void app_push_status(relay_status_t *st)
{
	char msg[40];
	os_memset(msg, 0, 40);

	if (st == NULL)
		st = &(ctrl_st.relay_status);

	os_sprintf(msg, "{\"r1\":%d,\"r2\":%d,\"r3\":%d,\"r4\":%d,\"r5\":%d}",
				st->r1,
				st->r2,
				st->r3,
				st->r4,
				st->r5
			);

	mjyun_publishstatus(msg);
	INFO("Pushed status = %s\r\n", msg);
}

irom void app_check_set_push_save(relay_status_t *st)
{
	if(st == NULL)
		return;

	if(ctrl_st.relay_status.r1 != st->r1 ||
		ctrl_st.relay_status.r2 != st->r2 ||
		ctrl_st.relay_status.r3 != st->r3 ||
		ctrl_st.relay_status.r4 != st->r4 ||
		ctrl_st.relay_status.r5 != st->r5) {

		INFO("saved the new status into flash when relay_status changed\r\n");
		ctrl_st.relay_status.r1 = st->r1;
		ctrl_st.relay_status.r2 = st->r2;
		ctrl_st.relay_status.r3 = st->r3;
		ctrl_st.relay_status.r4 = st->r4;
		ctrl_st.relay_status.r5 = st->r5;

		relay_set_status(NULL);
		app_push_status(NULL);
		param_save();
	}
}
