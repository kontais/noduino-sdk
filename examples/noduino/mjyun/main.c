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
#include "noduino.h"
#include "mjyun.h"

#define	DEBUG	1

irom void mjyun_stated_cb(MJYUN_State_t state)
{
    if (MJYUN_State() != state)
        os_printf("Platform: mjyun_state error \r\n");

    switch (state)
    {
        case WIFI_IDLE:
            os_printf("Platform: WIFI_IDLE\r\n");
            break;
        case WIFI_SMARTLINK_LINKING:
            os_printf("Platform: WIFI_SMARTLINK_LINKING\r\n");
            break;
        case WIFI_SMARTLINK_FINDING:
            os_printf("Platform: WIFI_SMARTLINK_FINDING\r\n");
            break;
        case WIFI_SMARTLINK_TIMEOUT:
            os_printf("Platform: WIFI_SMARTLINK_TIMEOUT\r\n");
            break;
        case WIFI_SMARTLINK_GETTING:
            os_printf("Platform: WIFI_SMARTLINK_GETTING\r\n");
            break;
        case WIFI_SMARTLINK_OK:
            os_printf("Platform: WIFI_SMARTLINK_OK\r\n");
            break;
        case WIFI_AP_OK:
            os_printf("Platform: WIFI_AP_OK\r\n");
            break;
        case WIFI_AP_ERROR:
            os_printf("Platform: WIFI_AP_ERROR\r\n");
            break;
        case WIFI_AP_STATION_OK:
            os_printf("Platform: WIFI_AP_STATION_OK\r\n");
            break;
        case WIFI_AP_STATION_ERROR:
            os_printf("Platform: WIFI_AP_STATION_ERROR\r\n");
            break;
        case WIFI_STATION_OK:
            os_printf("Platform: WIFI_STATION_OK\r\n");
            break;
        case WIFI_STATION_ERROR:
            os_printf("Platform: WIFI_STATION_ERROR\r\n");
            break;
        case MJYUN_CONNECTING:
            os_printf("Platform: MJYUN_CONNECTING\r\n");
            break;
        case MJYUN_CONNECTING_ERROR:
            os_printf("Platform: MJYUN_CONNECTING_ERROR\r\n");
            break;
        case MJYUN_CONNECTED:
            os_printf("Platform: MJYUN_CONNECTED \r\n");
			digitalWrite(2, LOW);
            break;
        case MJYUN_DISCONNECTED:
            os_printf("Platform: MJYUN_DISCONNECTED\r\n");
			digitalWrite(2, HIGH);
            break;
        default:
            break;
    }
}

irom void mjyun_receive(const char *event_name, const char *event_data)
{
	os_printf("RECEIVED: key:value [%s]:[%s]", event_name, event_data);
	// Publish back
	MJYUN_Publish(event_name, event_data);
}

irom void setup()
{
	serial_begin(115200);
	pinMode(2, OUTPUT);

	MJYUN_Init("gh_51111441aa63");
	MJYUN_StateChanged(mjyun_stated_cb);
    MJYUN_OnData(mjyun_receive);
}

irom void loop()
{
	serial_printf("Hello World!\r\n");
	delay(1500);
}
