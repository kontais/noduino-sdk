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

LOCAL void ICACHE_FLASH_ATTR
mjyun_stated_cb(mjyun_state_t state)
{
	if (mjyun_state() != state)
		INFO("Platform: mjyun_state error \r\n");

	switch (state) {
	case WIFI_IDLE:
		INFO("Platform: WIFI_IDLE\r\n");
		break;
	case WIFI_SMARTLINK_START:
		INFO("Platform: WIFI_SMARTLINK_START\r\n");
		app_set_smart_effect(0);
		break;

	case WIFI_SMARTLINK_LINKING:
		INFO("Platform: WIFI_SMARTLINK_LINKING\r\n");
		app_set_smart_effect(2);
		break;
	case WIFI_SMARTLINK_FINDING:
		INFO("Platform: WIFI_SMARTLINK_FINDING\r\n");
		app_set_smart_effect(0);
		break;
	case WIFI_SMARTLINK_TIMEOUT:
		INFO("Platform: WIFI_SMARTLINK_TIMEOUT\r\n");
		break;
	case WIFI_SMARTLINK_GETTING:
		INFO("Platform: WIFI_SMARTLINK_GETTING\r\n");
		app_set_smart_effect(1);
		break;
	case WIFI_SMARTLINK_OK:
		INFO("Platform: WIFI_SMARTLINK_OK\r\n");
		break;
	case WIFI_AP_OK:
		INFO("Platform: WIFI_AP_OK\r\n");
		break;
	case WIFI_AP_ERROR:
		INFO("Platform: WIFI_AP_ERROR\r\n");
		break;
	case WIFI_AP_STATION_OK:
		INFO("Platform: WIFI_AP_STATION_OK\r\n");
		break;
	case WIFI_AP_STATION_ERROR:
		INFO("Platform: WIFI_AP_STATION_ERROR\r\n");
		break;
	case WIFI_STATION_OK:
		INFO("Platform: WIFI_STATION_OK\r\n");
		break;
	case WIFI_STATION_ERROR:
		INFO("Platform: WIFI_STATION_ERROR\r\n");
		break;
	case MJYUN_CONNECTING:
		INFO("Platform: MJYUN_CONNECTING\r\n");
		break;
	case MJYUN_CONNECTING_ERROR:
		INFO("Platform: MJYUN_CONNECTING_ERROR\r\n");
		break;
	case MJYUN_CONNECTED:
		INFO("Platform: MJYUN_CONNECTED \r\n");
		break;
	case MJYUN_DISCONNECTED:
		INFO("Platform: MJYUN_DISCONNECTED\r\n");
		break;
	default:
		break;
	}
}

const mjyun_config_t mjyun_conf = {
	"gh_51111441aa63", /* 产品id [必填] (摩羯云管理界面添加的微信原始ID)*/
	"3708",/*产品子id(一般用于微信设备) [选填] (摩羯智能灯)*/
	"Hi, I'm coming!!!",/*设备上线时，给app发送online消息中的附加数据，[选填]*/
	"I will come back!!!",/*设备掉线时，给app发送offline消中的附加数据，[选填]*/
	NULL,
	WITH_MQTT
};

void ICACHE_FLASH_ATTR
platform_init(void)
{
	app_load();
	app_apply_settings();
	gpio16_output_conf();
	gpio16_output_set(1);

	mjyun_statechanged(mjyun_stated_cb);
	espnow_create();
	network_system_timer_callback_register(app_start_check);
	app_start_check(0);
	mjyun_ondata(mjyun_receive);
	mjyun_run(&mjyun_conf);
}

irom void system_init_done()
{
	INFO("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
	INFO("\r\nSDK version:%s\r\n", system_get_sdk_version());

	// Init platform
	platform_init();
}

irom void user_init()
{
#define DEV_MODE 1
#if defined(DEV_MODE)
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
#else
	// Set the port to print log info.
	UART_SetPrintPort(UART1);
	// Swap UART Port to RS485 bus
	system_uart_swap();
	// Set RS485 and LOG baudrate
	uart_init(BIT_RATE_9600, BIT_RATE_115200);
#endif

	// Wait serial port OK
	os_delay_us(100);

	// Set Wi-Fi mode
	wifi_set_opmode(STATIONAP_MODE);

	mjyun_setssidprefix("MJLIGHT_");

	mjpwm_cmd_t command = {
		.scatter = MJPWM_CMD_SCATTER_APDM,
		.frequency = MJPWM_CMD_FREQUENCY_DIVIDE_1,
		.bit_width = MJPWM_CMD_BIT_WIDTH_12,
		.reaction = MJPWM_CMD_REACTION_FAST,
		.one_shot = MJPWM_CMD_ONE_SHOT_DISABLE,
		.resv = 0,
	};

	mjpwm_init(PIN_DI, PIN_DCKI, command);

	system_init_done_cb(system_init_done);
}
