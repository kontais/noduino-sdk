/*
 *  Copyright (c) 2015 - 2025 MaiKe Labs
 *
 *  Http Client example
 *
 *  Rewritten by Jack Tan <jiankemeng@gmail.com>
 *  Based on great work from Martin d'Allens <martin.dallens@gmail.com>
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
#include "httpclient.h"
#include "user_interface.h"
#include "driver/uart.h"

#define AP_NAME               "YOUR_ROUTER_AP_NAME"
#define AP_PASS               "YOUR_ROUTER_AP_PASSWORD"

void wifi_enter_sta()
{
	struct station_config config;

	//Connect to the defined access point.
	wifi_station_get_config(&config);

	os_memset(&config, 0x0, sizeof(struct station_config));
	strcpy(config.ssid, AP_NAME);
	strcpy(config.password, AP_PASS);

	wifi_station_set_config(&config);
}

static ETSTimer test_timer;
static uint8_t test_count = 0;

static void ICACHE_FLASH_ATTR test_timer_cb()
{
	os_printf("\nTEST %d ", test_count);

	switch (test_count) {
	case 0:
		os_printf("=> IP address before network ready\n");
		http_get("http://173.194.45.65", "", http_callback_example);
		// This test will fail. The DHCP request returns directly, but the TCP connection hangs.
		// FIXME: wait for network to be ready before connecting?
		break;
	case 1:
		os_printf("=> Simple GET\n");
		http_get("http://wtfismyip.com/text", "",
			 http_callback_example);
		break;
	case 2:
		os_printf("=> Response too long\n");
		http_get("http://www.wikipedia.org/", "",
			 http_callback_example);
		break;
	case 3:
		os_printf("=> Connection refused\n");
		http_get("http://att.com:1234/", "", http_callback_example);
		break;
	case 4:
		os_printf("=> Empty response, wrong protocol\n");
		http_get("http://google.com:443/", "", http_callback_example);
		break;
	case 5:
		os_printf("=> Invalid DNS\n");
		http_get("http://invalid.dns/", "", http_callback_example);
		break;
	case 6:
		os_printf("=> Connection time out\n");
		http_get("http://google.com:1234/", "", http_callback_example);
		break;
	case 7:
		os_printf("=> Simple POST\n");
		http_post("http://httpbin.org/post",
			  "first_word=hello&second_word=world",
			  "Content-Type: application/x-www-form-urlencoded\r\n",
			  http_callback_example);
		break;
	case 8:
		os_printf("=> Moved\n");
		http_get("http://wikipedia.org/", "", http_callback_example);
		break;
	case 9:
		os_printf("=> IP address, 404\n");
		http_get("http://54.200.182.206/", "", http_callback_example);
		break;
	case 10:
		os_printf("=> Concurrent requests\n");
		http_get("http://wtfismyip.com/text", "",
			 http_callback_example);
		http_post("http://httpbin.org/post",
			  "first_word=hello&second_word=world",
			  "Content-Type: application/x-www-form-urlencoded\r\n",
			  http_callback_example);
		http_get("http://wtfismyip.com/text", "",
			 http_callback_example);
		http_post("http://httpbin.org/post",
			  "first_word=hello&second_word=world",
			  "Content-Type: application/x-www-form-urlencoded\r\n",
			  http_callback_example);
		http_get("http://wtfismyip.com/text", "",
			 http_callback_example);
		http_post("http://httpbin.org/post",
			  "first_word=hello&second_word=world",
			  "Content-Type: application/x-www-form-urlencoded\r\n",
			  http_callback_example);
		http_get("http://wtfismyip.com/text", "",
			 http_callback_example);
		// FIXME: this test sometimes fails with "DNS error code -1"
		break;
	default:
		os_printf("=> DONE\n");
		return;
	}
	test_count++;
	os_timer_arm(&test_timer, 3000, 0);
}

void ICACHE_FLASH_ATTR http_test()
{
	os_timer_disarm(&test_timer);
	os_timer_setfn(&test_timer, test_timer_cb, NULL);
	os_timer_arm(&test_timer, 0, 0);	// Start immediately.
}

//user_init is the user entry point of the Espressif SDK
void ICACHE_FLASH_ATTR user_init()
{
	//Initialize the uart0 and uart1 in 115200 bitrate
	uart_init(BIT_RATE_115200, BIT_RATE_115200);

	wifi_enter_sta();

	os_delay_us(100000);

	system_init_done_cb(http_test);
}
