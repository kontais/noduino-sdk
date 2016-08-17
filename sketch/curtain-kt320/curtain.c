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

void curtain_open()
{
	INFO("curtain open switch\n");
#if 1
	digitalWrite(D3, HIGH);
	delayMicroseconds(100000);
	digitalWrite(D3, LOW);
#else
	gpio_output_set(BIT13, 0, BIT13, 0);
	os_delay_us(100000);
	gpio_output_set(0, BIT13, BIT13, 0);
#endif
}

void curtain_close()
{
	INFO("curtain close switch\n");
#if 1
	digitalWrite(D4, HIGH);
	delayMicroseconds(100000);
	digitalWrite(D4, LOW);
#else
	gpio_output_set(BIT12, 0, BIT12, 0);
	os_delay_us(100000);
	gpio_output_set(0, BIT12, BIT12, 0);
#endif
}

void curtain_stop()
{
	INFO("curtain stop\n");
	switch (param_get_status())
	{
   		case 0:	
			// re-enter to stop the close process
			// need to check the runing state!
			curtain_close();
			break;
		case 2:
			// re-enter to stop the open process
			// need to check the runing state!
			curtain_open();
			break;
	}
}

irom void curtain_init()
{
	pinMode(D3, OUTPUT);
	pinMode(D4, OUTPUT);

	digitalWrite(D3, LOW);
	digitalWrite(D4, LOW);
}

void curtain_set_status(int status, int pos)
{
	int delta;

	// only need to process the stop status
	if(status == 1) {
		INFO("set curtain stop\r\n");
		curtain_stop();
		param_set_status(1);
	} else {
		delta = pos - param_get_position();
		if(delta > 0) {
			// open
			curtain_open();
			INFO("set curtain open (100%)\r\n");
			param_set_status(2);
		} else if (delta < 0) {
			// close
			curtain_close();
			INFO("set curtain close (0%)\r\n");
			param_set_status(0);
		}
	}

	param_set_position(pos);
	param_save();
}

void curtain_set_status_and_publish(int status, int pos)
{
	char msg[32];
	os_memset(msg, 0, 32);

	curtain_set_status(status, pos);

	os_sprintf(msg, "{\"status\":%d,\"position\":%d}", status, pos);
	mjyun_publishstatus(msg);
}

void curtain_publish_status()
{
	char msg[32];
	os_memset(msg, 0, 32);

	int status = param_get_status();
	int pos = param_get_position();

	os_sprintf(msg, "{\"status\":%d,\"position\":%d}", status, pos);
	mjyun_publishstatus(msg);
}
