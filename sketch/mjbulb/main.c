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
#include "main.h"

#define	DEBUG	1

void gpio16_output()
{
	GPF16 = GP16FFS(GPFFS_GPIO(16));	//Set mode to GPIO
	GPC16 = 0;
	GP16E |= 1;
}

void gpio16_high()
{
	GP16O |= 1;
}

void gpio16_low()
{
	GP16O &= ~1;
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
    os_printf("RECEIVED: key=%s,value=%s\r\n", event_name, event_data);
    // Publish back
    mjyun_publish(event_name, event_data);
}

void mjyun_connected()
{
    mjyun_publishstatus("{state:\"online\"}");
	gpio16_high();
}

void mjyun_disconnected()
{
	gpio16_low();
}

/*
 * 3707 --> 摩羯插座
 * 3708 --> 摩羯灯
 */
const mjyun_config_t mjyun_conf = {
    //"WotP0123456789",     /* 产品id [必填] */
    "gh_51111441aa63",      /* 产品id [必填] */
    "3708",                 /* 产品子id (一般用于微信设备) [选填]*/
    "Hi, I'm coming!!!",    /* 设备上线时，给app发送 online 消息中的附加数据，[选填] */
    "I will come back!!!"   /* 设备掉线时，给app发送 offline 消息中的附加数据，[选填] */
};

void init_yun()
{
    gpio16_output_conf();
    gpio16_output_set(1);
    mjyun_statechanged(mjyun_stated_cb);
    mjyun_ondata(mjyun_receive);
    mjyun_onconnected(mjyun_connected);
    mjyun_ondisconnected(mjyun_disconnected);

    mjyun_run(&mjyun_conf);
}

void main(int argc, char *argv[])
{
#ifdef DEBUG
	uart_init(115200, 115200);
#endif

	//Initialize the GPIO subsystem.
	gpio_init();

	//Set GPIO2 to output mode
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

    //gpio16 output
    gpio16_output();
    gpio16_low();

	//Set GPIO12 to output mode
	//PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);

	//Set GPIO2 low
	//GPIO_OUTPUT_SET(2, 0);
	gpio_output_set(BIT2, 0, BIT2, 0);

	system_init_done_cb(init_yun);
}
