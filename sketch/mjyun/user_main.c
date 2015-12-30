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
#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "gpio.h"

#include "noduino/esp8266_peri.h"
#include "driver/uart.h"
#include "mjyun.h"

#define	DEBUG	1

void switch_on()
{
#ifdef DEBUG
	os_printf("set gpio2 to low\n");
#endif
	gpio_output_set(0, BIT2, BIT2, 0);
}

void switch_off()
{
#ifdef DEBUG
	os_printf("set gpio2 to high\n");
#endif
	gpio_output_set(BIT2, 0, BIT2, 0);
}

void switch_init()
{
	gpio_init();

	//PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
}

static void mjyun_stated_cb(mjyun_state_t state)
{
    if (mjyun_state() != state)
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
            break;
        case MJYUN_DISCONNECTED:
            os_printf("Platform: MJYUN_DISCONNECTED\r\n");
            break;
        default:
            break;
    }
}

void mjyun_receive(const char *event_name, const char *event_data)
{
	os_printf("RECEIVED: key:value [%s]:[%s]", event_name, event_data);
	if(os_strncmp(event_data, "on", 2) == 0)
	{
#ifdef DEBUG
		os_printf("set switch on\r\n");
#endif
		switch_on();
	}
	if(os_strncmp(event_data, "off", 3) == 0)
	{
#ifdef DEBUG
		os_printf("set switch off\r\n");
#endif
		switch_off();
	}
	
	// Publish back
	mjyun_publish(event_name, event_data);
}

void mjyun_connected()
{
    mjyun_publishstatus("{state:\"online\"}");
}

void mjyun_disconnected()
{
}

void cos_check_ip()
{

	mjyun_statechanged(mjyun_stated_cb);
	mjyun_ondata(mjyun_receive);
	mjyun_onconnected(mjyun_connected);
	mjyun_ondisconnected(mjyun_disconnected);
	mjyun_init("gh_51111441aa63", NULL);
	//mjyun_init("WotP0123456789", NULL);
}

void user_init(void)
{
#ifdef DEBUG
	uart_init(115200, 115200);
#endif

	switch_init();

	os_printf("\r\nSystem started ...\r\n");
	system_init_done_cb(cos_check_ip);
}
