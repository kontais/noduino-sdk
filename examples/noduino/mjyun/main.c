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
#include "compile.h"

#define	DEBUG	1

irom void mjyun_stated_cb(mjyun_state_t state)
{
	if (mjyun_state() != state)
		os_printf("Platform: mjyun_state error \r\n");

	switch (state) {
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

void switch_on()
{
	os_printf("set gpio2 to low\n");
	digitalWrite(2, LOW);
}

void switch_off()
{
	os_printf("set gpio2 to high\n");
	digitalWrite(2, HIGH);
}

irom void mjyun_receive(const char *event_name, const char *event_data)
{
	os_printf("RECEIVED: key:value [%s]:[%s]", event_name, event_data);
	if (os_strncmp(event_data, "on", 2) == 0) {
		os_printf("set switch on\r\n");
		switch_on();
	}
	if (os_strncmp(event_data, "off", 3) == 0) {
		os_printf("set switch off\r\n");
		switch_off();
	}
	if (os_strncmp(event_data, "ota", 3) == 0) {
		os_printf("OTA: upgrade the firmware!\r\n");
		mjyun_mini_ota_start("ota/demo/files");
	}
}

void mjyun_connected()
{
	mjyun_publishstatus("off");
}

void mjyun_disconnected()
{

}

/*
 * 4285 --> 摩羯窗帘
 * 3707 --> 摩羯插座
 * 8636 --> 摩羯插座2
 * 3708 --> 摩羯灯
 * 6287 --> 传感器
 */
const mjyun_config_t mjyun_conf = {
	"gh_51111441aa63",		/* 产品id [必填] */
	"8636",					/* 产品子id (一般用于微信设备) [选填] */
	FW_VERSION,	/* 设备上线时，给app发送 online 消息中的附加数据，[选填] */
	"I will come back!!!"	/* 设备掉线时，给app发送 offline 消息中的附加数据，[选填] */
};

void init_yun()
{
	mjyun_statechanged(mjyun_stated_cb);
	mjyun_ondata(mjyun_receive);
	mjyun_onconnected(mjyun_connected);
	mjyun_ondisconnected(mjyun_disconnected);

	mjyun_run(&mjyun_conf);
	os_printf("Welcome to MJYUN ... \r\n");
	os_printf("%s", noduino_banner);
}

irom void setup()
{
	uart_init(115200, 115200);

	pinMode(2, OUTPUT);
	digitalWrite(2, HIGH);

	init_yun();
}

irom void loop()
{
	os_printf("MJYUN device heart beat!\r\n");
	delay(2000);
}
